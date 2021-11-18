// Fill out your copyright notice in the Description page of Project Settings.


#include "GamePhaseTrigger.h"
#include "Neokaiju/Core/PhasedGameMode.h"
#include "Engine/World.h"


void AGamePhaseTrigger::OnBeginTrigger(AActor* TriggeringActor)
{
	if (this->bShouldChangePhaseOnEnter)
	{
		this->ChangePhase();
	}
}

void AGamePhaseTrigger::OnEndTrigger(AActor* TriggeringActor)
{
	if (this->bShouldChangePhaseOnExit)
	{
		this->ChangePhase();
	}
}

void AGamePhaseTrigger::ChangePhase()
{
	APhasedGameMode* GameMode = Cast<APhasedGameMode>(this->GetWorld()->GetAuthGameMode());
	if (IsValid(GameMode))
	{
		switch (this->PhaseToTrigger)
		{
		case EGameModePhase::PrePlayPhase:
			GameMode->BeginPrePlayPhase();
			break;
		case EGameModePhase::PlayPhase:
			GameMode->BeginPlayPhase();
			break;
		case EGameModePhase::PostPlayPhase:
			GameMode->BeginPostPlayPhase(this->PlayResultToTrigger);
			break;
		case EGameModePhase::None:
		default:
			break;
		}
	}
}
