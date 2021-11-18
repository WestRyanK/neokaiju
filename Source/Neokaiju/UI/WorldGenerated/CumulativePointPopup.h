// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Engine/World.h"
#include <Engine.h>


#include "CumulativePointPopup.generated.h"

/**
 * 
 */
UCLASS()
class NEOKAIJU_API UCumulativePointPopup : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite)
		int multiplier = 1;
	UPROPERTY(BlueprintReadWrite)
		float score = 0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FVector2D playerOffset;
	UPROPERTY(BlueprintReadWrite)
		FVector2D positionInViewport;
	UFUNCTION()
		void Setup(int currentMultiplier, float PropertyValue);
	UFUNCTION(BlueprintImplementableEvent)
		void AddToScore(float addedScore);
	UFUNCTION(BlueprintImplementableEvent)
		void StylizedRemoveFromViewport();
protected:
	UFUNCTION(BlueprintCallable)
		void SetPositionAbovePlayer();
	UFUNCTION(BlueprintCallable)
		void ResetHUDActivePointPopup();
};
