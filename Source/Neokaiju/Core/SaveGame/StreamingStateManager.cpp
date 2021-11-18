// Fill out your copyright notice in the Description page of Project Settings.


#include "StreamingStateManager.h"
#include "Neokaiju/Core/SaveGame/NeoKaijuSaveGameSubsystem.h"
#include "Neokaiju/Core/SaveGame/SaveGameStateManager.h"
#include "Engine/World.h"

AStreamingStateManager::AStreamingStateManager()
{
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AStreamingStateManager::BeginPlay()
{
	Super::BeginPlay();

	LoadLevelState();
}

void AStreamingStateManager::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	this->SaveLevelState();
	Super::EndPlay(EndPlayReason);
}

void AStreamingStateManager::SaveLevelState()
{
	UNeoKaijuSaveGameSubsystem* SaveGameSubsystem = this->GetWorld()->GetSubsystem<UNeoKaijuSaveGameSubsystem>();
	//UNeoKaijuSaveGameSubsystem* SaveGameSubsystem = this->GetGameInstance()->GetSubsystem<UNeoKaijuSaveGameSubsystem>();
	if (IsValid(SaveGameSubsystem->SaveGameStateManager) && IsValid(SaveGameSubsystem->CurrentSaveGame))
	{
		float StartTime = this->GetWorld()->TimeSeconds;
		TArray<AActor*> Empty;
		SaveGameSubsystem->SaveGameStateManager->SaveLevelState(this, SaveGameSubsystem->CurrentSaveGame, this->GetLevel(), Empty);
		float EndTime = this->GetWorld()->TimeSeconds;
		float Duration = EndTime - StartTime;
		UE_LOG(LogTemp, Display, TEXT("Level '%s' state saved in %f seconds"), *this->GetLevel()->GetOuter()->GetName(), Duration);
	}
}

void AStreamingStateManager::LoadLevelState()
{
	UNeoKaijuSaveGameSubsystem* SaveGameSubsystem = this->GetWorld()->GetSubsystem<UNeoKaijuSaveGameSubsystem>();
	//UNeoKaijuSaveGameSubsystem* SaveGameSubsystem = this->GetGameInstance()->GetSubsystem<UNeoKaijuSaveGameSubsystem>();
	SaveGameSubsystem->LoadGameFromSlot();
	if (IsValid(SaveGameSubsystem->SaveGameStateManager) && IsValid(SaveGameSubsystem->CurrentSaveGame))
	{
		float StartTime = this->GetWorld()->TimeSeconds;
		TArray<AActor*> Empty;
		SaveGameSubsystem->SaveGameStateManager->LoadLevelState(this, SaveGameSubsystem->CurrentSaveGame, this->GetLevel(), Empty);
		float EndTime = this->GetWorld()->TimeSeconds;
		float Duration = EndTime - StartTime;
		UE_LOG(LogTemp, Display, TEXT("Level '%s' state loaded in %f seconds"), *this->GetLevel()->GetOuter()->GetName(), Duration);
	}
}

