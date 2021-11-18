// Fill out your copyright notice in the Description page of Project Settings.


#include "LaserDamageCAR.h"
#include "Neokaiju/Structures/BreakableStructures/BreakableStructureAttributeSet.h"

float ULaserDamageCAR::GetMinimumLevelToDamage(UAbilitySystemComponent* TargetASC) const
{
	return TargetASC->GetNumericAttribute(UBreakableStructureAttributeSet::GetMinimumKaijuLevelToLaserDamageAttribute());
}
