// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "NeokaijuMetaSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class NEOKAIJU_API UNeokaijuMetaSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
		FString CurrentSaveGameSlot;

	UPROPERTY(EditAnywhere)
		TArray<FString> AllSaveGameSlots;

};
