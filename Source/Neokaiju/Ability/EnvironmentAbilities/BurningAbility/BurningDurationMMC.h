// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "BurningDurationMMC.generated.h"

/**
 *
 */
UCLASS()
class NEOKAIJU_API UBurningDurationMMC : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		float MinBurningDurationSec = 3;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		float MaxBurningDurationSec = 6;

	UFUNCTION()
		float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const;
};
