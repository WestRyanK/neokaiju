// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseDamageCAR.h"
#include "Neokaiju/KaijuCharacter/KaijuAttributeSet.h"
#include "Neokaiju/Structures/BreakableStructures/BreakableStructureAttributeSet.h"

bool UBaseDamageCAR::CanApplyGameplayEffect_Implementation(const UGameplayEffect* GameplayEffect, const FGameplayEffectSpec& Spec, UAbilitySystemComponent* TargetASC) const
{
	UAbilitySystemComponent* SourceASC = Spec.GetEffectContext().GetInstigatorAbilitySystemComponent();
	float KaijuLevel = SourceASC->GetNumericAttribute(UKaijuAttributeSet::GetKaijuLevelAttribute());
	float MinimumLevelToDamage = this->GetMinimumLevelToDamage(TargetASC);

	bool bMeetsMinimumToDamage = (KaijuLevel >= MinimumLevelToDamage);
	bool bDoesTargetResistDamage = TargetASC->HasMatchingGameplayTag(this->ResistDamageTag);
	bool bMeetsOverrideLevel = KaijuLevel >= this->OverrideResistKaijuLevel;

	return bMeetsOverrideLevel || (bMeetsMinimumToDamage && !bDoesTargetResistDamage);
}

float UBaseDamageCAR::GetMinimumLevelToDamage(UAbilitySystemComponent* TargetASC) const
{
	return 0;
}
