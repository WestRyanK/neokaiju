// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Neokaiju/Core/DamageableAttributeSet.h"
#include "BreakableStructureAttributeSet.generated.h"

/**
 * 
 */
UCLASS()
class NEOKAIJU_API UBreakableStructureAttributeSet : public UDamageableAttributeSet
{
	GENERATED_BODY()
	
public:
	UBreakableStructureAttributeSet();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
		FGameplayAttributeData Tier;
	ATTRIBUTE_ACCESSORS(UBreakableStructureAttributeSet, Tier)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
		FGameplayAttributeData MinimumKaijuLevelToDashDamage;
	ATTRIBUTE_ACCESSORS(UBreakableStructureAttributeSet, MinimumKaijuLevelToDashDamage)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
		FGameplayAttributeData MinimumKaijuLevelToLaserDamage;
	ATTRIBUTE_ACCESSORS(UBreakableStructureAttributeSet, MinimumKaijuLevelToLaserDamage)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
		FGameplayAttributeData MinimumKaijuLevelToGrab;
	ATTRIBUTE_ACCESSORS(UBreakableStructureAttributeSet, MinimumKaijuLevelToGrab)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
		FGameplayAttributeData MinimumKaijuLevelToPassthrough;
	ATTRIBUTE_ACCESSORS(UBreakableStructureAttributeSet, MinimumKaijuLevelToPassthrough)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
		FGameplayAttributeData StructureValue;
	ATTRIBUTE_ACCESSORS(UBreakableStructureAttributeSet, StructureValue)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
		FGameplayAttributeData RegenerationAmount;
	ATTRIBUTE_ACCESSORS(UBreakableStructureAttributeSet, RegenerationAmount)

	
};
