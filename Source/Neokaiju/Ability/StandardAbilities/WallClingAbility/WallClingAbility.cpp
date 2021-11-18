// Fill out your copyright notice in the Description page of Project Settings.



#include "WallClingAbility.h"
#include "Neokaiju/KaijuCharacter/Components/EffectTriggerLineTraceComponent.h"
#include "Neokaiju/KaijuCharacter/Components/KaijuMovementComponent.h"
#include "Neokaiju/Ability/Core/Tasks/TickDurationAbilityTask.h"
#include "Kismet/KismetMathLibrary.h"
#include "Curves/CurveFloat.h"
#include "Neokaiju/Core/SafeDirectionUtility.h"
#include "Neokaiju/KaijuCharacter/KaijuCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"


UWallClingAbility::UWallClingAbility()
{

}

void UWallClingAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!this->CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		this->EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	this->Character = Cast<AKaijuCharacter>(ActorInfo->AvatarActor.Get());

	if (this->CanWallClingOnSurface())
	{
		this->WallClingStartTime = this->GetWorld()->GetTimeSeconds();

		UTickDurationAbilityTask* SlowTask = UTickDurationAbilityTask::TickForDuration(this);
		SlowTask->OnDurationTick.AddUniqueDynamic(this, &UWallClingAbility::WallClingTick);
		SlowTask->ReadyForActivation();


		this->OnBeginWallCling();

		AB_LOG(TEXT("Starting Cling"));
	}
	else
	{
		this->EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
	}
}


bool UWallClingAbility::CanWallClingOnSurface() const
{
	if (!IsValid(this->Character))
	{
		return false;
	}
	if (!this->Character->FacingWallTrigger->bDidCurrentTraceHit || !this->Character->FacingWallTrigger->CurrentTraceHitResult.Component.IsValid())
	{
		return false;
	}

	bool bIsSurfaceSimulatingPhysics = this->Character->FacingWallTrigger->CurrentTraceHitResult.Component.Get()->IsSimulatingPhysics();
	bool bDoesSurfaceSimulatingPhysicsCheckPass = this->bCanWallClingOnPhysicsEnabled || !bIsSurfaceSimulatingPhysics;

	bool bIsInputInDirectionOfSurface = this->IsInputInDirectionOfSurface();
	bool bDoesInputInDirectionOfSurfaceCheckPass = !this->bRequireInputInWallDirectionToStart || bIsInputInDirectionOfSurface;
	
	return bDoesSurfaceSimulatingPhysicsCheckPass && bDoesInputInDirectionOfSurfaceCheckPass;
}

bool UWallClingAbility::IsInputInDirectionOfSurface() const
{	
	if (!this->Character->FacingWallTrigger->bDidCurrentTraceHit)
	{
		return false;
	}

	if (!this->Character->GetIsPlayerAiming())
	{
		return false;
	}

	float AimInDirectionOfWall = FVector::DotProduct(this->Character->GetPrimaryAimDirection(), -this->Character->FacingWallTrigger->CurrentTraceHitResult.ImpactNormal);
	
	return AimInDirectionOfWall > 0;
}

void UWallClingAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if (IsValid(this->Character))
	{
		AB_LOG(TEXT("Ending Cling"));
	}
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UWallClingAbility::WallClingTick(float DeltaTime)
{
	this->PitchCharacterToWall(DeltaTime);
	this->SlowCharacterDescent();

	if (this->GetShouldReleaseFromWall())
	{
		this->CancelAbility(this->CurrentSpecHandle, this->CurrentActorInfo, this->CurrentActivationInfo, true);
	}
}

bool UWallClingAbility::GetShouldReleaseFromWall() const
{
	if (this->ReleaseWallClingAfterSeconds > 0)
	{
		float TimeElapsed = this->GetTimeSinceStart();
		return TimeElapsed >= this->ReleaseWallClingAfterSeconds;
	}
	else
	{
		return false;
	}
}

void UWallClingAbility::PitchCharacterToWall(float DeltaTime)
{
	// This version keeps our head up no matter what.
	FVector ImpactNormal = this->Character->FacingWallTrigger->CurrentTraceHitResult.ImpactNormal;
	FVector RightVector = FVector::UpVector ^ ImpactNormal; // ^ is cross product
	FVector UpSlopeVector = ImpactNormal ^ RightVector;
	FVector UpFacingVector = USafeDirectionUtility::RotateVectorOnYAxisTowards(this->Character->CharacterRotationPoint->GetForwardVector(), UpSlopeVector, FVector::ZeroVector, FVector::ZeroVector, this->AdjustPitchSpeed, DeltaTime);

	FVector ForwardDirection = USafeDirectionUtility::GetSafeForwardDirectionFromFacing(this->Character->KaijuMovement->GetFacingDirection());
	this->Character->KaijuMovement->SetCharacterForward(
		UpFacingVector,
		ForwardDirection,
		FVector::UpVector,
		0.0f,
		0.0f,
		true);
}

void UWallClingAbility::SlowCharacterDescent()
{
	float DescentSpeed = FMath::Max(-this->GetCurrentMaxDescentSlideSpeed(), this->Character->GetCharacterMovement()->Velocity.Z);
	this->Character->GetCharacterMovement()->Velocity.Z = DescentSpeed;
}

float UWallClingAbility::GetCurrentMaxDescentSlideSpeed() const
{
	if (IsValid(this->MaxDescentSlideSpeedOverTime))
	{
		float TimeElapsed = this->GetTimeSinceStart();
		float CurrentMaxDescentSlideSpeed = this->MaxDescentSlideSpeedOverTime->GetFloatValue(TimeElapsed);
		return CurrentMaxDescentSlideSpeed;
	}
	else
	{
		return this->MaxDescentSlideSpeed;
	}
}

float UWallClingAbility::GetTimeSinceStart() const
{
	float TimeElapsed = this->GetWorld()->GetTimeSeconds() - this->WallClingStartTime;
	return TimeElapsed;
}
