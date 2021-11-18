// Fill out your copyright notice in the Description page of Project Settings.


#include "PhasedGameMode.h"
#include "Neokaiju/Core/NeokaijuGameInstance.h"
#include "Engine/World.h"

void APhasedGameMode::BeginPlay()
{
	Super::BeginPlay();
	this->BeginPrePlayPhase();

	UNeokaijuGameInstance* GameInstance = this->GetWorld()->GetGameInstance<UNeokaijuGameInstance>();
	GameInstance->LoadGameSettings();
}

EGameModePhase APhasedGameMode::GetCurrentPhase() const
{
	return this->CurrentPhase;
}

void APhasedGameMode::BeginPrePlayPhase()
{
	if (this->CurrentPhase != EGameModePhase::PrePlayPhase)
	{
		this->CurrentPhase = EGameModePhase::PrePlayPhase;
		this->OnPrePlayPhaseBegan.Broadcast();
		this->PrePlayPhaseBegan();
	}
}

void APhasedGameMode::PrePlayPhaseBegan_Implementation()
{
	this->BeginPlayPhase();
}

void APhasedGameMode::BeginPlayPhase()
{
	if (this->CurrentPhase != EGameModePhase::PlayPhase)
	{
		this->CurrentPhase = EGameModePhase::PlayPhase;
		this->OnPlayPhaseBegan.Broadcast();
		this->PlayPhaseBegan();
	}
}

void APhasedGameMode::BeginPostPlayPhase(EGameModePlayResult PlayResultValue)
{
	if (this->CurrentPhase != EGameModePhase::PostPlayPhase)
	{
		this->CurrentPhase = EGameModePhase::PostPlayPhase;
		this->PlayResult = PlayResultValue;
		this->OnPostPlayPhaseBegan.Broadcast(this->PlayResult);
		this->PostPlayPhaseBegan();
	}
}
