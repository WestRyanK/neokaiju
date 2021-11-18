// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveGameStateManager.h"
#include "Neokaiju/Core/SaveGame/NeokaijuSaveGame.h"
#include "Neokaiju/Core/SaveGame/SaveableInterface.h"
#include "Neokaiju/Core/SaveGame/GeneralSaveStateManager.h"
#include "Neokaiju/Core/SaveGame/BaseLevelSaveStateManager.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"
#include "Engine/Level.h"
#include "Engine/World.h"

// Saveable Objects Concept
// https://www.youtube.com/watch?v=YrdEha-Ogc8
void USaveGameStateManager::LoadGameState(UObject* WorldContextObject, UNeokaijuSaveGame* SaveGame)
{
	//this->CurrentlyLoadingSaveGame = SaveGame;
	//this->LoadMapHandle = FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &USaveGameStateManager::OnWorldLoaded);
	UGameplayStatics::OpenLevel(WorldContextObject, FName(*SaveGame->CurrentLevelName));
}

//void USaveGameStateManager::OnWorldLoaded(UWorld* LoadedWorld)
//{
	//this->GeneralSaveStateManager->LoadState(LoadedWorld, this->CurrentlyLoadingSaveGame);

	//TArray<AActor*> SaveableActors;
	//this->GetSaveableActorsInWorld(LoadedWorld, SaveableActors);

	//UNeokaijuSaveGame* SaveGame = this->CurrentlyLoadingSaveGame;
	//this->CurrentlyLoadingSaveGame = nullptr;
	//for (ULevel* Level : LoadedWorld->GetLevels())
	//{
	//	this->LoadLevelState(LoadedWorld, SaveGame, Level, SaveableActors);
	//}

	//FCoreUObjectDelegates::PostLoadMapWithWorld.Remove(this->LoadMapHandle);
//}

void USaveGameStateManager::LoadLevelState(UObject* WorldContextObject, UNeokaijuSaveGame* SaveGame, ULevel* Level, TArray<AActor*>& SaveableActors)
{
	//if (IsValid(this->CurrentlyLoadingSaveGame))
	//{
	//	return;
	//}

	UWorld* World = WorldContextObject->GetWorld();
	FString LevelName = Level->GetOuter()->GetName();
	if (IsValid(World))
	{
		if (SaveableActors.Num() == 0)
		{
			this->GetSaveableActorsInWorld(World, SaveableActors);
		}

		TArray<TScriptInterface<ISaveableInterface>> SaveablesInLevel;
		this->FilterToSaveablesInLevel(SaveableActors, SaveablesInLevel, Level);

		for (UBaseLevelSaveStateManager* Manager : this->LevelSaveStateManagers)
		{
			Manager->LoadStateInLevel(SaveGame, LevelName, SaveablesInLevel);
		}
	}
}

void USaveGameStateManager::SaveGameState(UObject* WorldContextObject, UNeokaijuSaveGame* SaveGame)
{
	UWorld* World = WorldContextObject->GetWorld();
	if (IsValid(World))
	{
		FString LevelName = UGameplayStatics::GetCurrentLevelName(WorldContextObject);
		SaveGame->CurrentLevelName = LevelName;

		this->GeneralSaveStateManager->SaveState(WorldContextObject, SaveGame);

		TArray<AActor*> SaveableActors;
		this->GetSaveableActorsInWorld(World, SaveableActors);

		for (ULevel* Level : World->GetLevels())
		{
			this->SaveLevelState(World, SaveGame, Level, SaveableActors);
		}
	}
}

void USaveGameStateManager::SaveLevelState(UObject* WorldContextObject, UNeokaijuSaveGame* SaveGame, ULevel* Level, TArray<AActor*>& SaveableActors)
{
	UWorld* World = WorldContextObject->GetWorld();
	FString LevelName = Level->GetOuter()->GetName();
	if (IsValid(World))
	{
		if (SaveableActors.Num() == 0)
		{
			this->GetSaveableActorsInWorld(World, SaveableActors);
		}

		TArray<TScriptInterface<ISaveableInterface>> SaveablesInLevel;
		this->FilterToSaveablesInLevel(SaveableActors, SaveablesInLevel, Level);

		for (UBaseLevelSaveStateManager* Manager : this->LevelSaveStateManagers)
		{
			Manager->SaveStateInLevel(SaveGame, LevelName, SaveablesInLevel);
		}
	}
}

void USaveGameStateManager::GetSaveableActorsInWorld(UWorld* World, TArray<AActor*>& OutSaveableActors)
{
	if (IsValid(World))
	{
		UGameplayStatics::GetAllActorsWithInterface(World, USaveableInterface::StaticClass(), OutSaveableActors);
	}
}

void USaveGameStateManager::FilterToSaveablesInLevel(TArray<AActor*> SaveableActors, TArray<TScriptInterface<ISaveableInterface>>& OutSaveablesInLevel, ULevel* Level)
{
	for (AActor* SaveableActor : SaveableActors)
	{
		if (!IsValid(Level) || SaveableActor->IsInLevel(Level))
		{
			OutSaveablesInLevel.Add(SaveableActor);
		}
	}
}
