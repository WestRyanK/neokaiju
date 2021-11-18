// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "IndividualHealthIndicator.generated.h"

/**
 * 
 */
UCLASS()
class NEOKAIJU_API UIndividualHealthIndicator : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
		void SetPercentage(float newValue);
protected:
	UPROPERTY(BlueprintReadOnly)
		float currentPercentage;
	UFUNCTION(BlueprintImplementableEvent)
		void RefreshUI();
};
