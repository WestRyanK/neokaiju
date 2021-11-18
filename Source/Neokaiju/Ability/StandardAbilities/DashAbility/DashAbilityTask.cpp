// Fill out your copyright notice in the Description page of Project Settings.


#include "DashAbilityTask.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "Neokaiju/Structures/InteractiveStructure.h"
#include "Neokaiju/Ability/Core/Abilities/BaseGameplayAbility.h"
#include "Components/CapsuleComponent.h"
#include "Engine/Engine.h"
#include "Curves/CurveFloat.h"
#include "Neokaiju/KaijuCharacter/KaijuCharacter.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Controller.h"


UDashAbilityTask* UDashAbilityTask::Dash(
	UGameplayAbility* OwningAbility,
	FVector DashDirection,
	float DashDistance,
	float DashDuration,
	bool bDoesAllowGlancingHitRedirect,
	float MinGlancingHitRedirectAngle,
	bool bDoesAllowOffCenterHitSliding,
	float MaxOffCenterClipHitSize,
	UCurveFloat* DashPositionOverTime,
	bool bContinueVelocityAfterDash)
{
	UDashAbilityTask* DashAbilityTask = NewAbilityTask<UDashAbilityTask>(OwningAbility);
	DashAbilityTask->MoveAlongCurveInDirection = DashDirection;
	DashAbilityTask->MoveAlongCurveOfLength = DashDistance;
	DashAbilityTask->MoveAlongCurveForDuration = DashDuration;
	DashAbilityTask->bDoesAllowGlancingHitRedirect = bDoesAllowGlancingHitRedirect;
	DashAbilityTask->MinGlancingHitRedirectAngle = MinGlancingHitRedirectAngle;
	DashAbilityTask->bDoesAllowOffCenterHitSliding = bDoesAllowOffCenterHitSliding;
	DashAbilityTask->MaxOffCenterClipHitSize = MaxOffCenterClipHitSize;
	DashAbilityTask->bContinueVelocityAfterDash = bContinueVelocityAfterDash;
	DashAbilityTask->PositionOverTime = DashPositionOverTime;
	DashAbilityTask->bIsMovementAbsoluteAlongCurve = true;
	DashAbilityTask->bDebugIsFirstHit = true;


	return DashAbilityTask;
}

void UDashAbilityTask::Activate()
{
	Super::Activate();

	this->CapsuleRadius = this->Character->GetCapsuleComponent()->GetScaledCapsuleRadius();
	this->MaxImpactAngleValidForClipHit = 90 - FMath::RadiansToDegrees(FMath::Acos((this->CapsuleRadius - this->MaxOffCenterClipHitSize) / this->CapsuleRadius));
}

void UDashAbilityTask::DashImpeded(FVector CharacterVelocity, FVector ImpactNormal, AActor* HitActor)
{
	this->EndTask();
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		this->OnDashImpeded.Broadcast(CharacterVelocity, ImpactNormal, HitActor);
	}
}

void UDashAbilityTask::DashHit(FVector CharacterVelocity, FVector ImpactNormal, AActor* HitActor)
{
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		this->OnDashHit.Broadcast(CharacterVelocity, ImpactNormal, HitActor);
	}
}

void UDashAbilityTask::OnDestroy(bool bInOwnerFinished)
{
	if (IsValid(this->Character))
	{

		if (this->bContinueVelocityAfterDash)
		{
			float FinalSpeed = this->GetDerivativeAtPercent(1.0f) * this->MoveAlongCurveOfLength;
			this->Character->GetCharacterMovement()->Velocity = this->MoveAlongCurveInDirection * FinalSpeed; // After Dash is finished keep character's dash velocity.
		}
		else
		{
			this->Character->GetCharacterMovement()->Velocity = FVector(0); // Cancel any previous velocity so the character stops immediately after a dash.
		}
	}
	Super::OnDestroy(bInOwnerFinished);
}

void UDashAbilityTask::OnHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
	FVector ImpactNormal = this->GetZXOnlyVector(Hit.ImpactNormal);
	if (OtherActor != this->GetAvatarActor())
	{
		float CharacterSpeedAtImpact = this->GetDerivativeAtPercent(this->GetPercentComplete()) * this->MoveAlongCurveOfLength;
		float ProgressAtImpact = this->GetProgressAtPercent(this->GetPercentComplete());
		if (CharacterSpeedAtImpact > 0.0f && ProgressAtImpact > 0.0f)
		{
			//ABTASK_LOG(TEXT("Hit something while dashing"));
			FVector CharacterVelocityAtImpact = CharacterSpeedAtImpact * this->MoveAlongCurveInDirection;
			//ABTASK_LOG(TEXT("Velocity at impact %s"), *CharacterVelocityAtImpact.ToString());
			this->DashHit(CharacterVelocityAtImpact, ImpactNormal, OtherActor);

			// By this point, if the hit block's health was low enough, the block has been destroyed by now. Therefore, the actor is invalid.
			// If it is not invalid, that means it was not destroyed, so we should react to it by bouncing off, or have our dash be redirected in case of a glancing blow.
			if (IsValid(OtherActor))
			{
				bool bWasClippingHit = this->SlideDashIfClippingHit(Hit);
				if (bWasClippingHit)
				{
					this->bDebugIsFirstHit = false;
					return;
				}
				bool bWasGlancingHit = this->RedirectDashIfGlancingHit(Hit);
				if (bWasGlancingHit)
				{
					this->bDebugIsFirstHit = false;
					return;
				}
				ABTASK_LOG(TEXT("Is still valid? %d"), IsValid(OtherActor));
				this->DashImpeded(CharacterVelocityAtImpact, ImpactNormal, OtherActor);

			}
		}
		else
		{
			ABTASK_LOG(TEXT("Ignoring reversed dash collision..."));
		}
	}
}

FVector UDashAbilityTask::GetZXOnlyVector(FVector Vector) const
{
	Vector.Y = 0;
	Vector.Normalize();
	return Vector;
}


bool UDashAbilityTask::RedirectDashIfGlancingHit(const FHitResult& Hit)
{
	FVector ImpactNormal = this->GetZXOnlyVector(Hit.ImpactNormal);
	float ImpactAngle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(ImpactNormal, -this->MoveAlongCurveInDirection.GetSafeNormal())));

	if (this->bDebugIsFirstHit)
	{
		ABTASK_LOG(TEXT("Impact Angle: %f"), ImpactAngle);
	}

	if (this->bDoesAllowGlancingHitRedirect && ImpactAngle >= this->MinGlancingHitRedirectAngle)
	{
		FVector RightVectorBetweenDashAndNormal = this->MoveAlongCurveInDirection ^ ImpactNormal;
		FVector SurfaceInDashDirection = ImpactNormal ^ RightVectorBetweenDashAndNormal;
		SurfaceInDashDirection.Normalize();
		if (this->bDebugIsFirstHit)
		{
			ABTASK_LOG(TEXT("ImpactNormal %s, Right %s, SurfaceDashDir %s"), *ImpactNormal.ToString(), *RightVectorBetweenDashAndNormal.ToString(), *SurfaceInDashDirection.ToString());
			ABTASK_LOG(TEXT("Glanced. Redirecting Dash from %s to %s"), *this->MoveAlongCurveInDirection.ToString(), *SurfaceInDashDirection.ToString());
		}

		this->MoveAlongCurveInDirection = SurfaceInDashDirection;

		this->ProgressAtLastDirectionChange = this->GetProgressAtPercent(this->GetPercentComplete());
		this->LocationAtLastDirectionChange = this->GetAvatarActor()->GetActorLocation();

		return true;
	}
	return false;
}

const float SMALL_ANGLE = 0.0f;

bool UDashAbilityTask::SlideDashIfClippingHit(const FHitResult& Hit)
{
	FVector ImpactNormal = this->GetZXOnlyVector(Hit.ImpactNormal);
	FVector DirectionOfImpactPoint = this->GetZXOnlyVector(Hit.ImpactPoint - this->Character->GetActorLocation()).GetSafeNormal();
	float OffCenterAngleRad = FMath::Acos(FVector::DotProduct(this->MoveAlongCurveInDirection.GetSafeNormal(), DirectionOfImpactPoint));
	float ClipSize = this->CapsuleRadius - FMath::Sin(OffCenterAngleRad) * this->CapsuleRadius;

	float ImpactAngle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(ImpactNormal, -this->MoveAlongCurveInDirection.GetSafeNormal())));
	float OffCenterAngleDegrees = FMath::RadiansToDegrees(OffCenterAngleRad);

	bool bIsValidSmallAngleClip = (ImpactAngle < this->MaxImpactAngleValidForClipHit && ClipSize < this->MaxOffCenterClipHitSize);
	bool bIsValidLargeAngleClip = (ImpactAngle >= this->MaxImpactAngleValidForClipHit && ImpactAngle < OffCenterAngleDegrees + SMALL_ANGLE);
	bool bIsValidClip = bIsValidSmallAngleClip || bIsValidLargeAngleClip;


	if (this->bDebugIsFirstHit)
	{
		ABTASK_LOG(TEXT("MaxImpactAngle: %f"), this->MaxImpactAngleValidForClipHit);
		ABTASK_LOG(TEXT("ClipSize: %f"), ClipSize);
		ABTASK_LOG(TEXT("ImpactAngle: %f"), ImpactAngle);
		ABTASK_LOG(TEXT("OffCenterAngle: %f"), OffCenterAngleDegrees);
		ABTASK_LOG(TEXT("SmallAngleClip: %d"), bIsValidSmallAngleClip);
		ABTASK_LOG(TEXT("LargeAngleClip: %d"), bIsValidLargeAngleClip);
	}

	FVector RightVectorBetweenDashAndImpact = this->MoveAlongCurveInDirection ^ DirectionOfImpactPoint;
	FVector TangentInDashDirection = DirectionOfImpactPoint ^ RightVectorBetweenDashAndImpact;
	TangentInDashDirection.Normalize();

	this->DrawDebugAngles(Hit, TangentInDashDirection, ClipSize);
	if (this->bDoesAllowOffCenterHitSliding && bIsValidClip)
	{

		float PercentComplete = this->GetPercentComplete();
		float DeltaTime = this->GetWorld()->GetDeltaSeconds();
		float DerivativeAtPercent = this->GetDerivativeAtPercent(PercentComplete, DeltaTime / this->MoveAlongCurveForDuration);
		float Offset = DerivativeAtPercent * this->MoveAlongCurveOfLength * DeltaTime / this->MoveAlongCurveForDuration;
		this->Character->AddActorWorldOffset(TangentInDashDirection * Offset, false);

		this->ProgressAtLastDirectionChange = this->GetProgressAtPercent(PercentComplete);
		this->LocationAtLastDirectionChange = this->GetAvatarActor()->GetActorLocation();

		//this->DrawDebugAngles(Hit, TangentInDashDirection, ClipSize);

		return true;
	}

	return false;
}

void UDashAbilityTask::DrawDebugAngles(const FHitResult& Hit, FVector TangentInDashDirection, float ClipSize)
{
	UBaseGameplayAbility* BaseAbility = Cast<UBaseGameplayAbility>(this->Ability);

	if (this->bDebugIsFirstHit && (!IsValid(BaseAbility) || BaseAbility->bIsAbilityDebugEnabled))
	{
		UE_LOG(LogTemp, Display, TEXT("Debug Draw"));
		FVector ActorLoc = this->Character->GetActorLocation();
		float duration = 8.0f;
		DrawDebugLine(this->GetWorld(), Hit.ImpactPoint, Hit.ImpactPoint + TangentInDashDirection * 100, FColor::Red, false, duration, 0, 2.0f);
		DrawDebugLine(this->GetWorld(), ActorLoc, ActorLoc + this->MoveAlongCurveInDirection * 100, FColor::Blue, false, duration, 0, 2.0f);
		DrawDebugLine(this->GetWorld(), Hit.ImpactPoint, ActorLoc, FColor::Green, false, duration, 0, 2.0f);
		DrawDebugLine(this->GetWorld(), Hit.ImpactPoint, Hit.ImpactPoint + Hit.ImpactNormal * 100, FColor::Purple, false, duration, 0, 2.0f);
		DrawDebugCircle(this->GetWorld(), ActorLoc, 40.0f, 32, FColor::Orange, false, duration, 0, 2.0f, FVector::ForwardVector, FVector::UpVector, false);

		FVector CircleBottom = ActorLoc + this->CapsuleRadius * FVector::DownVector;
		DrawDebugLine(this->GetWorld(), CircleBottom, CircleBottom + FVector::UpVector * ClipSize, FColor::Magenta, false, duration, 0, 2.0f);
	}
}
