// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Neokaiju/Environment/BaseTrigger.h"
#include "Camera/PlayerCameraManager.h"
#include "EasingTrigger.generated.h"

/**
 * 
 */
UCLASS()
class NEOKAIJU_API AEasingTrigger : public ABaseTrigger
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Trigger")
		float EaseInTime;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Trigger")
		float EaseOutTime;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Trigger")
		TEnumAsByte<EViewTargetBlendFunction> EaseInBlendFunction = EViewTargetBlendFunction::VTBlend_Linear;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Trigger")
		TEnumAsByte<EViewTargetBlendFunction> EaseOutBlendFunction = EViewTargetBlendFunction::VTBlend_Linear;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Trigger")
		float EaseInBlendExponent;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Trigger")
		float EaseOutBlendExponent;
};
