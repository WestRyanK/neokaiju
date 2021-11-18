// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include "Subsystems/GameInstanceSubsystem.h"
#include "Subsystems/WorldSubsystem.h"
#include "Neokaiju/Core/SaveGame/NeokaijuSaveGame.h"
#include "Neokaiju/Core/SaveGame/NeokaijuMetaSaveGame.h"
#include "NeoKaijuSaveGameSubsystem.generated.h"

class USaveGameStateManager;
class UHighScoreSave;
class AKaijuScoreState;
/**
 *
 */
UCLASS()
class NEOKAIJU_API UNeoKaijuSaveGameSubsystem : public UWorldSubsystem
//class NEOKAIJU_API UNeoKaijuSaveGameSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()


public:
	virtual void Initialize(FSubsystemCollectionBase& Collection);

	UPROPERTY(BlueprintReadOnly)
		UNeokaijuSaveGame* CurrentSaveGame = nullptr;

	UPROPERTY(BlueprintReadOnly)
		UHighScoreSave* HighScoreSave = nullptr;

	UPROPERTY(BlueprintReadOnly)
		USaveGameStateManager* SaveGameStateManager;

	UFUNCTION(BlueprintCallable)
		void SaveGameToSlot(FString SlotName = TEXT(""));

	UFUNCTION(BlueprintCallable)
		void LoadGameFromSlot(FString SlotName = TEXT(""));

	UFUNCTION(BlueprintCallable)
		void DeleteGameInSlot(FString SlotName = TEXT(""));

	UFUNCTION(BlueprintCallable)
		TArray<FString> GetSavedGameSlots();

	UFUNCTION(BlueprintCallable)
		FString GetCurrentSaveGameSlot();

	UFUNCTION(BlueprintCallable)
		void SetCurrentSaveGameSlot(FString CurrentSlotName);

	UFUNCTION(BlueprintCallable)
		void SaveHighScore();

	UFUNCTION(BlueprintCallable)
		void LoadHighScore();

	UFUNCTION(BlueprintCallable)
		void UpdateHighScore(AKaijuScoreState* KaijuScoreState);

	UFUNCTION(BlueprintCallable)
		void ClearHighScore();

private:

	UPROPERTY()
		UNeokaijuMetaSaveGame* MetaSaveGame = nullptr;

	UFUNCTION()
		void LoadMetaSaveGame();

	UFUNCTION()
		void SaveMetaSaveGame();

	UFUNCTION()
		void DeleteSlotFromList(FString SlotName);

	UFUNCTION()
		bool IsValidSlotName(FString SlotName);

	static const FString META_SAVE_GAME_SLOT_NAME;

	static const FString HIGH_SCORE_SAVE_GAME_SLOT_NAME;

};
