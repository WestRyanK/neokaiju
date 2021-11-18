// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Neokaiju/Core/SaveGame/BaseLevelSaveStateManager.h"
#include "KaijuCharacterSaveState.generated.h"


class UNeokaijuSaveGame;
class ISaveableInterface;

/**
 *
 */
USTRUCT()
struct FKaijuCharacterSaveState
{
	GENERATED_BODY()
public:
	UPROPERTY()
	FVector KaijuLocation;
	UPROPERTY()
	FRotator KaijuRotation;
	UPROPERTY()
	float KaijuLevel;
	UPROPERTY()
	float Health;
	UPROPERTY()
	float MaxHealth;
	UPROPERTY()
	float LaserDamage;
	UPROPERTY()
	float LaserCharge;
	UPROPERTY()
	float MaxLaserCharge;
	UPROPERTY()
	float DashDamage;
	UPROPERTY()
	float DashDistance;
	UPROPERTY()
	float DashDuration;
	UPROPERTY()
	float JumpHeight;
	UPROPERTY()
	float WalkSpeed;
	UPROPERTY()
	float KaijuSizeScale;
};

UCLASS()
class NEOKAIJU_API UKaijuCharacterSaveStateManager : public UBaseLevelSaveStateManager
{
	GENERATED_BODY()
public:
	virtual void SaveStateInLevel(UNeokaijuSaveGame* SaveGame, FString LevelName, TArray<TScriptInterface<ISaveableInterface>>& SaveablesInLevel) override;

	virtual void LoadStateInLevel(UNeokaijuSaveGame* SaveGame, FString LevelName, TArray<TScriptInterface<ISaveableInterface>>& SaveablesInLevel) override;
};
