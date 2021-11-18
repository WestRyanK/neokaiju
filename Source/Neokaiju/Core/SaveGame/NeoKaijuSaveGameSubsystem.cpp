// Fill out your copyright notice in the Description page of Project Settings.


#include "NeoKaijuSaveGameSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Neokaiju/Core/SaveGame/SaveGameStateManager.h"
#include "Neokaiju/Core/SaveGame/SaveableInterface.h"
#include "Neokaiju/Core/SaveGame/GeneralSaveStateManager.h"
#include "Neokaiju/Core/SaveGame/HighScoreSave.h"
#include "Neokaiju/Structures/BreakableStructures/BreakableStructureSaveState.h"
#include "Neokaiju/KaijuCharacter/KaijuCharacterSaveState.h"
#include "Neokaiju/KaijuCharacter/KaijuScoreStateSaveState.h"
#include "Engine/World.h"

FString const UNeoKaijuSaveGameSubsystem::META_SAVE_GAME_SLOT_NAME = TEXT("MetaSaveGame");

FString const UNeoKaijuSaveGameSubsystem::HIGH_SCORE_SAVE_GAME_SLOT_NAME = TEXT("HighScoreSaveGame");

void UNeoKaijuSaveGameSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	this->SaveGameStateManager = NewObject<USaveGameStateManager>(this);
	this->SaveGameStateManager->LevelSaveStateManagers.Add(NewObject<UBreakableStructureSaveStateManager>(this));
	this->SaveGameStateManager->LevelSaveStateManagers.Add(NewObject<UKaijuCharacterSaveStateManager>(this));
	this->SaveGameStateManager->LevelSaveStateManagers.Add(NewObject<UKaijuScoreStateSaveStateManager>(this));
	this->SaveGameStateManager->GeneralSaveStateManager = NewObject<UGeneralSaveStateManager>(this);
	this->LoadGameFromSlot();
}

void UNeoKaijuSaveGameSubsystem::SaveGameToSlot(FString SlotName)
{
	if (SlotName.IsEmpty())
	{
		SlotName = this->GetCurrentSaveGameSlot();
	}

	if (!this->IsValidSlotName(SlotName))
		return;

	UGameplayStatics::SaveGameToSlot(this->CurrentSaveGame, SlotName, 0);
	this->SetCurrentSaveGameSlot(SlotName);
}

void UNeoKaijuSaveGameSubsystem::LoadGameFromSlot(FString SlotName)
{
	if (SlotName.IsEmpty())
	{
		SlotName = this->GetCurrentSaveGameSlot();
		if (SlotName.IsEmpty())
		{
			this->CurrentSaveGame = Cast<UNeokaijuSaveGame>(UGameplayStatics::CreateSaveGameObject(UNeokaijuSaveGame::StaticClass()));
			this->SaveGameToSlot(SlotName);
		}
	}


	if (!this->IsValidSlotName(SlotName))
		return;

	this->CurrentSaveGame = Cast<UNeokaijuSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));

	if (!IsValid(this->CurrentSaveGame))
	{
		this->CurrentSaveGame = Cast<UNeokaijuSaveGame>(UGameplayStatics::CreateSaveGameObject(UNeokaijuSaveGame::StaticClass()));
		this->SaveGameToSlot(SlotName);
	}

	this->SetCurrentSaveGameSlot(SlotName);
}

void UNeoKaijuSaveGameSubsystem::DeleteGameInSlot(FString SlotName)
{
	if (SlotName.IsEmpty())
	{
		SlotName = this->GetCurrentSaveGameSlot();
	}

	if (!this->IsValidSlotName(SlotName))
		return;

	if (SlotName == this->GetCurrentSaveGameSlot())
	{
		this->CurrentSaveGame = Cast<UNeokaijuSaveGame>(UGameplayStatics::CreateSaveGameObject(UNeokaijuSaveGame::StaticClass()));
		this->SetCurrentSaveGameSlot(TEXT(""));
	}

	UGameplayStatics::DeleteGameInSlot(SlotName, 0);

	this->DeleteSlotFromList(SlotName);
}

void UNeoKaijuSaveGameSubsystem::LoadMetaSaveGame()
{
	this->MetaSaveGame = Cast<UNeokaijuMetaSaveGame>(UGameplayStatics::LoadGameFromSlot(UNeoKaijuSaveGameSubsystem::META_SAVE_GAME_SLOT_NAME, 0));
	if (!IsValid(this->MetaSaveGame))
	{
		this->MetaSaveGame = Cast<UNeokaijuMetaSaveGame>(UGameplayStatics::CreateSaveGameObject(UNeokaijuMetaSaveGame::StaticClass()));
		this->SaveMetaSaveGame();
	}
}

void UNeoKaijuSaveGameSubsystem::SaveMetaSaveGame()
{
	UGameplayStatics::SaveGameToSlot(this->MetaSaveGame, UNeoKaijuSaveGameSubsystem::META_SAVE_GAME_SLOT_NAME, 0);
}

TArray<FString> UNeoKaijuSaveGameSubsystem::GetSavedGameSlots()
{
	this->LoadMetaSaveGame();
	return this->MetaSaveGame->AllSaveGameSlots;
}

FString UNeoKaijuSaveGameSubsystem::GetCurrentSaveGameSlot()
{
	this->LoadMetaSaveGame();
	return this->MetaSaveGame->CurrentSaveGameSlot;
}

void UNeoKaijuSaveGameSubsystem::SetCurrentSaveGameSlot(FString CurrentSlotName)
{
	this->LoadMetaSaveGame();
	this->MetaSaveGame->CurrentSaveGameSlot = CurrentSlotName;
	if (!CurrentSlotName.IsEmpty())
	{
		this->MetaSaveGame->AllSaveGameSlots.AddUnique(CurrentSlotName);
	}
	this->SaveMetaSaveGame();
}

void UNeoKaijuSaveGameSubsystem::DeleteSlotFromList(FString SlotName)
{
	this->LoadMetaSaveGame();
	this->MetaSaveGame->AllSaveGameSlots.Remove(SlotName);
	this->SaveMetaSaveGame();
}

bool UNeoKaijuSaveGameSubsystem::IsValidSlotName(FString SlotName)
{
	bool bIsMetaSlotName = (SlotName == UNeoKaijuSaveGameSubsystem::META_SAVE_GAME_SLOT_NAME);
	bool bIsHighScoreSlotName = (SlotName == UNeoKaijuSaveGameSubsystem::HIGH_SCORE_SAVE_GAME_SLOT_NAME);
	bool bIsEmpty = SlotName.IsEmpty();
	return (!bIsEmpty && !bIsMetaSlotName && !bIsHighScoreSlotName);
}


void UNeoKaijuSaveGameSubsystem::SaveHighScore()
{
	UGameplayStatics::SaveGameToSlot(this->HighScoreSave, UNeoKaijuSaveGameSubsystem::HIGH_SCORE_SAVE_GAME_SLOT_NAME, 0);
}

void UNeoKaijuSaveGameSubsystem::LoadHighScore()
{
	this->HighScoreSave = Cast<UHighScoreSave>(UGameplayStatics::LoadGameFromSlot(UNeoKaijuSaveGameSubsystem::HIGH_SCORE_SAVE_GAME_SLOT_NAME, 0));
	if (!IsValid(this->HighScoreSave))
	{
		this->HighScoreSave = Cast<UHighScoreSave>(UGameplayStatics::CreateSaveGameObject(UHighScoreSave::StaticClass()));
	}
}

void UNeoKaijuSaveGameSubsystem::UpdateHighScore(AKaijuScoreState* KaijuScoreState)
{
	if (!IsValid(this->HighScoreSave))
	{
		this->LoadHighScore();
	}

	this->HighScoreSave->Update(KaijuScoreState);

	this->SaveHighScore();
}

void UNeoKaijuSaveGameSubsystem::ClearHighScore()
{
	UGameplayStatics::DeleteGameInSlot(UNeoKaijuSaveGameSubsystem::HIGH_SCORE_SAVE_GAME_SLOT_NAME, 0);
}
