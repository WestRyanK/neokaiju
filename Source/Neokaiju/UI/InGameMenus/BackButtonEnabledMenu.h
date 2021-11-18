// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Neokaiju/UI/NavigatableMenu.h"
#include "BackButtonEnabledMenu.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBackButtonPressed);

/**
 * 
 */
UCLASS()
class NEOKAIJU_API UBackButtonEnabledMenu : public UNavigatableMenu
{
	GENERATED_BODY()
	

public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

protected:
	UFUNCTION(BlueprintCallable)
		void BackButtonPressed();

	UFUNCTION()
		void BackHandler();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void OnBackPressed();

	FInputActionBinding BackBinding;
};
