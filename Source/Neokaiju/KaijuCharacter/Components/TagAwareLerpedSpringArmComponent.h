// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Neokaiju/KaijuCharacter/Components/LerpedSpringArmComponent.h"
#include "GameplayTagContainer.h"
#include "TagAwareLerpedSpringArmComponent.generated.h"

/**
 * 
 */
UCLASS()
class NEOKAIJU_API UTagAwareLerpedSpringArmComponent : public ULerpedSpringArmComponent
{
	GENERATED_BODY()

public:

	// If any of these tags are present on the owning actor, the SpringArm will not extend.
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FGameplayTagContainer ExtendBlockingParentTags;

	// If any of these tags are present on the owning actor, the SpringArm will not retract.
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FGameplayTagContainer RetractBlockingParentTags;
	
protected:
	virtual float GetLerpSpeed(ELerpedSpringArmMotion Motion);
};
