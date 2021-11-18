// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GeneralSaveStateManager.generated.h"

class UNeokaijuSaveGame;
/**
 *
 */
UCLASS()
class NEOKAIJU_API UGeneralSaveStateManager : public UObject
{
	GENERATED_BODY()


public:
	UFUNCTION()
		virtual void SaveState(UObject* WorldContextObject, UNeokaijuSaveGame* SaveGame);

	UFUNCTION()
		virtual void LoadState(UObject* WorldContextObject, UNeokaijuSaveGame* SaveGame);
};
