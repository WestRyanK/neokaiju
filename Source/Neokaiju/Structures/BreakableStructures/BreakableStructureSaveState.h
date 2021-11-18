// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Neokaiju/Core/SaveGame/BaseLevelSaveStateManager.h"
#include "BreakableStructureSaveState.generated.h"

class UNeokaijuSaveGame;
class ISaveableInterface;
/**
 *
 */
USTRUCT()
struct FBreakableStructureSaveState
{
	GENERATED_BODY()
public:
	UPROPERTY()
		FVector StructureLocation;
	UPROPERTY()
		FRotator StructureRotation;
	UPROPERTY()
		float StructureHealth;
	UPROPERTY()
		bool bIsSimulatingPhysics;
};

UCLASS()
class NEOKAIJU_API UBreakableStructureSaveStateManager : public UBaseLevelSaveStateManager
{
	GENERATED_BODY()
public:
	virtual void SaveStateInLevel(UNeokaijuSaveGame* SaveGame, FString LevelName, TArray<TScriptInterface<ISaveableInterface>>& SaveablesInLevel) override;

	virtual void LoadStateInLevel(UNeokaijuSaveGame* SaveGame, FString LevelName, TArray<TScriptInterface<ISaveableInterface>>& SaveablesInLevel) override;
};
