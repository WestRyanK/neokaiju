// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuParent.generated.h"

class UNavigatableMenu;
/**
 * 
 */
UCLASS()
class NEOKAIJU_API UMenuParent : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void AddMenu(UNavigatableMenu* Menu);
	
};
