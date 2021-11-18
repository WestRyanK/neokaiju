// Fill out your copyright notice in the Description page of Project Settings.


#include "DashDamageCAR.h"
#include "Neokaiju/Structures/BreakableStructures/BreakableStructureAttributeSet.h"

float UDashDamageCAR::GetMinimumLevelToDamage(UAbilitySystemComponent* TargetASC) const
{
	return TargetASC->GetNumericAttribute(UBreakableStructureAttributeSet::GetMinimumKaijuLevelToDashDamageAttribute());}
