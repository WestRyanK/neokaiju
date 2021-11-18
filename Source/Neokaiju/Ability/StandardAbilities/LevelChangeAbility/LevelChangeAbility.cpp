// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelChangeAbility.h"
#include "Neokaiju/KaijuCharacter/KaijuCharacter.h"
#include "Abilities/Tasks/AbilityTask_WaitDelay.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Neokaiju/KaijuCharacter/Components/CameraPanComponent.h"

ULevelChangeAbility::ULevelChangeAbility()
{
	this->bDoesAbilityActivateOnGranted = true;
}

void ULevelChangeAbility::OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	AKaijuCharacter* CharacterActor = Cast<AKaijuCharacter>(ActorInfo->AvatarActor);
	if (IsValid(CharacterActor))
	{
		//if (ULevelChangeAbility::OriginalCameraFocusPosition == FVector::ZeroVector)
		//{
		//	ULevelChangeAbility::OriginalCameraFocusPosition = CharacterActor->CameraFocusPoint->GetRelativeLocation();
		//	//ULevelChangeAbility::OriginalCameraFocusPosition = CharacterActor->CameraPanComponent->GetRelativeLocation();
		//}
	}
	Super::OnAvatarSet(ActorInfo, Spec);
}

void ULevelChangeAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!this->CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		this->EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);


	AKaijuCharacter* Character = Cast<AKaijuCharacter>(ActorInfo->AvatarActor.Get());
	if (IsValid(Character))
	{
		OldCharacterLevel = Character->GetKaijuLevel();
		NewCharacterLevel = Character->GetKaijuLevel();
		float AbilityLevel = this->GetAbilityLevel();
		if (AbilityLevel == -1.0f)
		{
			AB_LOG(TEXT("Leveling Up!"));
			NewCharacterLevel = OldCharacterLevel + 1;
		}
		else if (AbilityLevel == -2.0f)
		{
			AB_LOG(TEXT("Leveling Down!"));
			NewCharacterLevel = OldCharacterLevel - 1;
		}
		else
		{
			NewCharacterLevel = AbilityLevel;
		}

		this->ApplyLevel();
		Character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
		this->OnPreLevelChange(OldCharacterLevel, NewCharacterLevel);
		UE_LOG(LogTemp, Display, TEXT("PreLevelChange"));
		

		UAbilityTask_WaitDelay* Delay = UAbilityTask_WaitDelay::WaitDelay(this, this->SlomoDuration * this->SlowdownMagnitude);
		Delay->OnFinish.AddUniqueDynamic(this, &ULevelChangeAbility::OnPost);
		Delay->ReadyForActivation();
	}
	else 
	{
		this->EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
	}

	
}

void ULevelChangeAbility::ApplyLevel()
{
	this->ApplyGameplayEffectToOwner(this->CurrentSpecHandle, this->CurrentActorInfo, this->CurrentActivationInfo, this->LevelChangeEffect.GetDefaultObject(), NewCharacterLevel);
	UE_LOG(LogTemp, Display, TEXT("EffectApplied"));

	this->UpdateSpecialLevelEffect();

	//UAbilityTask_WaitDelay* OnPostDelay = UAbilityTask_WaitDelay::WaitDelay(this, this->SlowTimeAfterSizeChange * this->SlowdownMagnitude);
	//OnPostDelay->OnFinish.AddUniqueDynamic(this, &ULevelChangeAbility::OnPost);
	//OnPostDelay->ReadyForActivation();
}

void ULevelChangeAbility::UpdateSpecialLevelEffect()
{
	// This didn't work because the handle isn't kept due to GameplayAbility Instancing
	//if (this->CurrentSpecialLevelEffectHandle.IsValid())
	//{
	//	this->GetAbilitySystemComponentFromActorInfo()->RemoveActiveGameplayEffect(this->CurrentSpecialLevelEffectHandle);
	//	this->CurrentSpecialLevelEffectHandle.Invalidate();
	//}

	TSubclassOf<UGameplayEffect>* SpecialLevelEffect = this->SpecialLevelEffects.Find(this->NewCharacterLevel);
	if (SpecialLevelEffect)
	{
		this->CurrentSpecialLevelEffectHandle = this->ApplyGameplayEffectToOwner(this->CurrentSpecHandle, this->CurrentActorInfo, this->CurrentActivationInfo, SpecialLevelEffect->GetDefaultObject(), 0);
	}
}

void ULevelChangeAbility::OnPost()
{
	
	this->OnPostLevelChange(OldCharacterLevel, NewCharacterLevel);
	this->ApplyGameplayEffectToOwner(this->CurrentSpecHandle, this->CurrentActorInfo, this->CurrentActivationInfo, this->PostLevelChangeEffect.GetDefaultObject(), NewCharacterLevel);
	AKaijuCharacter* Character = Cast<AKaijuCharacter>(this->GetAvatarActorFromActorInfo());
	if (IsValid(Character))
	{
		Character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Falling);
	}
	UE_LOG(LogTemp, Display, TEXT("PostLevelChange"));
	this->EndAbility(this->CurrentSpecHandle, this->CurrentActorInfo, this->CurrentActivationInfo, true, false);
}
