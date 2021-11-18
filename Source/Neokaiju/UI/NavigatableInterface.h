// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Blueprint/UserWidget.h"
#include "NavigatableInterface.generated.h"


// This class does not need to be modified.
UINTERFACE(MinimalAPI, Blueprintable)
class UNavigatableInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 *
 */
class NEOKAIJU_API INavigatableInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		UWidget* GetInitiallyFocusedWidget();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void GetFocusableWidgets(TArray<UWidget*>& OutFocusableWidgets);
};
