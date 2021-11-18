// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SaveGameStateManager.generated.h"

class AActor;
class UWorld;
class ISaveableInterface;
class UNeokaijuSaveGame;
class ULevel;
class UBaseLevelSaveStateManager;
class UGeneralSaveStateManager;
/**
 *
 */
UCLASS()
class NEOKAIJU_API USaveGameStateManager : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"))
		void LoadGameState(UObject* WorldContextObject, UNeokaijuSaveGame* SaveGame);

	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"))
		void LoadLevelState(UObject* WorldContextObject, UNeokaijuSaveGame* SaveGame, ULevel* Level, TArray<AActor*>& SaveableActors);

	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"))
		void SaveGameState(UObject* WorldContextObject, UNeokaijuSaveGame* SaveGame);

	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"))
		void SaveLevelState(UObject* WorldContextObject, UNeokaijuSaveGame* SaveGame, ULevel* Level, TArray<AActor*>& SaveableActors);

	UPROPERTY()
		TSet<UBaseLevelSaveStateManager*> LevelSaveStateManagers;

	UPROPERTY()
		UGeneralSaveStateManager* GeneralSaveStateManager;


//protected:
//	UPROPERTY()
//		UNeokaijuSaveGame* CurrentlyLoadingSaveGame;

private:
	//FDelegateHandle LoadMapHandle;
	//
	//UFUNCTION()
	//	void OnWorldLoaded(UWorld* LoadedWorld);

	UFUNCTION()
		void GetSaveableActorsInWorld(UWorld* World, TArray<AActor*>& OutSaveableActors);

	void FilterToSaveablesInLevel(TArray<AActor*> SaveableActors, TArray<TScriptInterface<ISaveableInterface>>& OutSaveablesInLevel, ULevel* Level);
};
