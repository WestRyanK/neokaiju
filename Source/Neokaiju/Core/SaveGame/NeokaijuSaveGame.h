// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Neokaiju/KaijuCharacter/KaijuCharacterSaveState.h"
#include "Neokaiju/KaijuCharacter/KaijuScoreStateSaveState.h"
#include "Neokaiju/Structures/BreakableStructures/BreakableStructureSaveState.h"
#include "NeokaijuSaveGame.generated.h"

USTRUCT()
struct FNeokaijuSaveGameLevel
{
	GENERATED_BODY()

public:
	UPROPERTY()
		TMap<FString, FBreakableStructureSaveState> BreakableStructures;
	UPROPERTY()
		TMap<FString, FKaijuCharacterSaveState> KaijuCharacters;
	UPROPERTY()
		TMap<FString, FKaijuScoreStateSaveState> KaijuScoreStates;
};
/**
 *
 */
UCLASS()
class NEOKAIJU_API UNeokaijuSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY()
		TMap<FString, FNeokaijuSaveGameLevel> SavedLevels;

	UPROPERTY()
		FString CurrentLevelName = TEXT("LevelFinal_Main");

	UPROPERTY(EditAnywhere)
		float MusicVolume = 1.0f;
	
	UPROPERTY(EditAnywhere)
		float SoundVolume = 1.0f;
};
