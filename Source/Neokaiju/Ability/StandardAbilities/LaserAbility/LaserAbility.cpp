// Fill out your copyright notice in the Description page of Project Settings.




#include "LaserAbility.h"
#include "Neokaiju/Ability/StandardAbilities/LaserAbility/FireLaserAbilityTask.h"
#include "Neokaiju/Ability/Core/Tasks/InputReleaseOrDelayAbilityTask.h"
#include "Neokaiju/KaijuCharacter/KaijuCharacter.h"
#include "Neokaiju/Ability/StandardAbilities/LaserAbility/LaserCameraAbilityTask.h"
#include "Neokaiju/KaijuCharacter/Components/KaijuMovementComponent.h"
#include "Particles/ParticleSystem.h"
#include "Sound/SoundBase.h"
#include "Camera/CameraShake.h"
#include "GameFramework/CharacterMovementComponent.h"

ULaserAbility::ULaserAbility()
{

}

void ULaserAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!this->CommitCheck(Handle, ActorInfo, ActivationInfo))
	{
		this->EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	this->Character = Cast<AKaijuCharacter>(ActorInfo->AvatarActor.Get());
	if (IsValid(this->Character))
	{
		bStartedInAir = this->Character->GetCharacterMovement()->IsFalling();

		this->Character->GetCharacterMovement()->MovementMode = EMovementMode::MOVE_None;

		this->OnChargeLaserStart(this->LaserSocketName, this->Character->GetMesh());
		UInputReleaseOrDelayAbilityTask* Task = UInputReleaseOrDelayAbilityTask::WaitInputReleaseOrDelay(
			this,
			this->ChargeLaserDuration);
		Task->OnTick.AddUniqueDynamic(this, &ULaserAbility::ChargeLaserTick);
		Task->OnComplete.AddUniqueDynamic(this, &ULaserAbility::ChargeLaserComplete);
		Task->OnCancel.AddUniqueDynamic(this, &ULaserAbility::ChargeLaserCancel);
		Task->ReadyForActivation();
	}
}

bool ULaserAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	// This is important! Super checks to see if cooldown is still happening! If we don't have this, it will ignore cooldown.
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}

	return true;
}

bool ULaserAbility::CommitAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	// This is important! 
	// Super checks CanActivateAbility and consequently checks Cooldown.
	if (!Super::CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		return false;
	}

	return true;
}

void ULaserAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if (IsValid(this->Character))
	{
		this->Character->GetCharacterMovement()->MovementMode = EMovementMode::MOVE_Walking;
	}

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	this->OnFireLaserComplete();
}

void ULaserAbility::ChargeLaserTick(float TimeRemaining)
{
	this->OnChargeLaserTick(TimeRemaining);
}

void ULaserAbility::ChargeLaserComplete(float TimeRemaining)
{
	AB_LOG(TEXT("Laser Charged"));
	this->OnChargeLaserComplete();

	if (IsValid(this->Character))
	{
		this->InputPressed(this->CurrentSpecHandle, this->CurrentActorInfo, this->CurrentActivationInfo);


		int KaijuLevel = (int)(this->Character->GetKaijuLevel());
		float LaserRadius = 0.0f;
		if (this->LaserRadiusPerLevel.IsValidIndex(KaijuLevel))
		{
			LaserRadius = this->LaserRadiusPerLevel[KaijuLevel];
		}

		float LaserDepthDistance = this->Character->GetScaledCollisionDepthDistance();

		UFireLaserAbilityTask* FireLaserTask = UFireLaserAbilityTask::FireLaser(
			this,
			this->LaserDamageEffect,
			this->LaserOverlapEffect,
			this->LaserDamageRate,
			this->CostCommitRate,
			this->LaserAimSpeed,
			this->LaserExtendSpeed,
			this->Character->GetLaserLength(),//this->MaxLaserDistance, // TODO replace with Laser values
			this->LaserSocketName,
			this->Character->GetMesh(),
			this->LaserTraceChannel,
			LaserRadius,
			this->LaserImpactRadius,
			LaserDepthDistance);
		FireLaserTask->OnFireLaserComplete.AddUniqueDynamic(this, &ULaserAbility::FireLaserComplete);
		FireLaserTask->OnFireLaserTick.AddUniqueDynamic(this, &ULaserAbility::FireLaserTick);

		this->LaserCameraTask = ULaserCameraAbilityTask::LaserCamera(
			this,
			this->FireLaserCameraShake,
			this->LaserCameraOffset,
			this->bShouldCenterCameraOnActor);

		this->OnFireLaserStart(this->LaserSocketName, this->Character->GetMesh());
		FireLaserTask->ReadyForActivation();
		this->LaserCameraTask->ReadyForActivation();
	}
	else
	{
		this->EndAbility(this->CurrentSpecHandle, this->CurrentActorInfo, this->CurrentActivationInfo, true, true);
	}
}

void ULaserAbility::ChargeLaserCancel(float TimeRemaining)
{
	AB_LOG(TEXT("Laser Charge Canceled"));
	this->OnChargeLaserCancel();
	this->EndAbility(this->CurrentSpecHandle, this->CurrentActorInfo, this->CurrentActivationInfo, true, true);
}

void ULaserAbility::FireLaserComplete(FVector LaserAimDirection, FHitResult LaserHit)
{
	AB_LOG(TEXT("Laser Fire Completed"));
	if (IsValid(this->LaserCameraTask))
	{
		this->LaserCameraTask->FireLaserComplete();
	}
	this->OnFireLaserComplete();
	this->EndAbility(this->CurrentSpecHandle, this->CurrentActorInfo, this->CurrentActivationInfo, true, false);
}

void ULaserAbility::FireLaserTick(FVector LaserAimDirection, FHitResult LaserHit) {
	if (IsValid(this->LaserCameraTask))
	{
		this->LaserCameraTask->SetLaserAimDirection(LaserAimDirection);
	}

	this->Character->KaijuMovement->SetFacingDirection(USafeDirectionUtility::GetFacingDirectionFromAxisValue(LaserAimDirection.X));
	this->OnFireLaserTick(LaserAimDirection, LaserHit);
}
