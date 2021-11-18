// Fill out your copyright notice in the Description page of Project Settings.


#include "RegenerateAbility.h"
#include "Neokaiju/KaijuCharacter/KaijuCharacter.h"
#include "Neokaiju/Structures/BreakableStructures/BreakableStructureSubsystem.h"
#include "Neokaiju/Structures/BreakableStructures/BreakableStructure.h"
#include "Neokaiju/Structures/BreakableStructures/BreakableStructureAttributeSet.h"
#include "Neokaiju/Core/PlayLevelGameMode.h"
#include "Engine/World.h"

void URegenerateAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!this->CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		this->EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UBreakableStructureSubsystem* Subsystem = this->GetWorld()->GetSubsystem<UBreakableStructureSubsystem>();
	Subsystem->OnStructureDestroyed.AddUniqueDynamic(this, &URegenerateAbility::OnStructureDestroyed);
}

void URegenerateAbility::OnStructureDestroyed(ABreakableStructure* DestroyedStructure)
{
	APlayLevelGameMode* GameMode = Cast<APlayLevelGameMode>(this->GetWorld()->GetAuthGameMode());
	if (IsValid(GameMode) && GameMode->CanScoreBeChanged())
	{
		if (IsValid(DestroyedStructure))
		{
			float AmountToAdd = DestroyedStructure->AttributeSet->GetRegenerationAmount();
			this->AddToCurrentAmount(AmountToAdd);
		}
	}
}

void URegenerateAbility::SetCurrentAmount(float CurrentAmountValue)
{
	Super::SetCurrentAmount(CurrentAmountValue);
	
	//UKaijuCharacterEventSubsystem* Subsystem = this->GetWorld()->GetSubsystem<UKaijuCharacterEventSubsystem>();
	//Subsystem->KaijuHealthRegenerationChanged(this->CurrentAmount, this->OverflowAmount);
}
