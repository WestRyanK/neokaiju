// Fill out your copyright notice in the Description page of Project Settings.


#include "BurningDurationMMC.h"

float UBurningDurationMMC::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	float BurningDuration = FMath::FRandRange(this->MinBurningDurationSec, this->MaxBurningDurationSec);

	return BurningDuration;
}
