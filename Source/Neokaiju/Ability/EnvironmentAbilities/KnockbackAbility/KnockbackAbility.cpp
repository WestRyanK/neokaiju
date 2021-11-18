// Fill out your copyright notice in the Description page of Project Settings.


#include "KnockbackAbility.h"
#include "Neokaiju/KaijuCharacter/KaijuCharacter.h"

UKnockbackAbility::UKnockbackAbility()
{
	this->bDoesAbilityActivateOnGranted = true;
}

void UKnockbackAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!this->CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		this->EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	
	AKaijuCharacter* KaijuCharacter = Cast<AKaijuCharacter>(ActorInfo->AvatarActor.Get());

	if (IsValid(KaijuCharacter)) {
		FVector forwardVector = KaijuCharacter->CharacterRotationPoint->GetForwardVector();
		FVector verticalVector = FVector(0, 0, 100);
		FVector horizontalVector = FVector(forwardVector.X * 100, forwardVector.Y * 100, 0);
		KaijuCharacter->LaunchCharacter(-horizontalVector*HorizontalVelocityMultiplier + verticalVector*VerticalVelocityMultiplier, true, true);
		UE_LOG(LogTemp, Display, TEXT("Taking Knockback"));
	}

	EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
}