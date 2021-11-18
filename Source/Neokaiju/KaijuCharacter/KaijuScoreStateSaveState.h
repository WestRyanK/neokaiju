// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Neokaiju/Core/SaveGame/BaseLevelSaveStateManager.h"
#include "KaijuScoreStateSaveState.generated.h"

class UNeokaijuSaveGame;
class ISaveableInterface;

/**
 *
 */
USTRUCT()
struct FKaijuScoreStateSaveState
{
	GENERATED_BODY()
public:
	UPROPERTY()
		float PropertyDamageDealt = 0.0f;
	UPROPERTY()
		float BabyKaijusSaved = 0.0f;
	UPROPERTY()
		float WeaponsDestroyed = 0.0f;
	UPROPERTY()
		float TimeBonusMultiplier = 1.0f;
	UPROPERTY()
		float BonusRemainingTime = 1.0f;
	UPROPERTY()
		float BlocksDestroyedForBonus = 1.0f;
};

UCLASS()
class NEOKAIJU_API UKaijuScoreStateSaveStateManager : public UBaseLevelSaveStateManager
{
	GENERATED_BODY()
public:
	virtual void SaveStateInLevel(UNeokaijuSaveGame* SaveGame, FString LevelName, TArray<TScriptInterface<ISaveableInterface>>& SaveablesInLevel) override;

	virtual void LoadStateInLevel(UNeokaijuSaveGame* SaveGame, FString LevelName, TArray<TScriptInterface<ISaveableInterface>>& SaveablesInLevel) override;
};
