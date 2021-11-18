// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuGameMode.h"
#include "Neokaiju/Core/NeokaijuGameInstance.h"
#include "Engine/World.h"

void AMainMenuGameMode::BeginPlay()
{
	Super::BeginPlay();

	UNeokaijuGameInstance* GameInstance = this->GetWorld()->GetGameInstance<UNeokaijuGameInstance>();
	GameInstance->LoadGameSettings();
}
