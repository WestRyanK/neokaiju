// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BaseLevelSaveStateManager.generated.h"

class UNeokaijuSaveGame;
class ISaveableInterface;
/**
 *
 */
UCLASS()
class NEOKAIJU_API UBaseLevelSaveStateManager : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION()
		virtual void SaveStateInLevel(UNeokaijuSaveGame* SaveGame, FString LevelName, TArray<TScriptInterface<ISaveableInterface>>& SaveablesInLevel) { };

	UFUNCTION()
		virtual void LoadStateInLevel(UNeokaijuSaveGame* SaveGame, FString LevelName, TArray<TScriptInterface<ISaveableInterface>>& SaveablesInLevel) { };
};
