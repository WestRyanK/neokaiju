// Fill out your copyright notice in the Description page of Project Settings.


#include "PassivelyDestroyCAR.h"
#include "Neokaiju/KaijuCharacter/KaijuAttributeSet.h"
#include "Neokaiju/Structures/BreakableStructures/BreakableStructureAttributeSet.h"

bool UPassivelyDestroyCAR::CanApplyGameplayEffect_Implementation(const UGameplayEffect* GameplayEffect, const FGameplayEffectSpec& Spec, UAbilitySystemComponent* TargetASC) const
{
	UAbilitySystemComponent* SourceASC = Spec.GetEffectContext().GetInstigatorAbilitySystemComponent();
	float KaijuLevel = SourceASC->GetNumericAttribute(UKaijuAttributeSet::GetKaijuLevelAttribute());
	float MinimumKaijuLevelToPassivelyDestroy = TargetASC->GetNumericAttribute(UBreakableStructureAttributeSet::GetMinimumKaijuLevelToPassthroughAttribute());

	return (KaijuLevel >= MinimumKaijuLevelToPassivelyDestroy);
}

