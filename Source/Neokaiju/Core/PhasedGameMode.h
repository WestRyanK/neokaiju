// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PhasedGameMode.generated.h"

UENUM(BlueprintType)
enum class EGameModePlayResult : uint8
{
	None		UMETA(DisplayName = "None"),
	Cancel		UMETA(DisplayName = "Cancel"),
	Success		UMETA(DisplayName = "Success"),
	Failure		UMETA(DisplayName = "Failure"),
	Timeout		UMETA(DisplayName = "Timeout"),
	EarlyExit	UMETA(DisplayName = "EarlyExit")
};

UENUM(BlueprintType)
enum class EGameModePhase : uint8
{
	None				UMETA(DisplayName = "None"),
	PrePlayPhase		UMETA(DisplayName = "PrePlayPhase"),
	PlayPhase			UMETA(DisplayName = "PlayPhase"),
	PostPlayPhase		UMETA(DisplayName = "PostPlayPhase")
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPrePlayPhaseBegan);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayPhaseBegan);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPostPlayPhaseBegan, EGameModePlayResult, PlayResult);

/**
 * 
 */
UCLASS()
class NEOKAIJU_API APhasedGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay();

	UFUNCTION(BlueprintCallable, BlueprintPure)
		EGameModePhase GetCurrentPhase() const;

	UPROPERTY(BlueprintAssignable)
		FOnPrePlayPhaseBegan OnPrePlayPhaseBegan;

	UPROPERTY(BlueprintAssignable)
		FOnPlayPhaseBegan OnPlayPhaseBegan;

	UPROPERTY(BlueprintAssignable)
		FOnPostPlayPhaseBegan OnPostPlayPhaseBegan;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		EGameModePlayResult PlayResult = EGameModePlayResult::None;

	UFUNCTION(BlueprintCallable)
		virtual void BeginPrePlayPhase();

	UFUNCTION(BlueprintCallable)
		virtual void BeginPlayPhase();

	UFUNCTION(BlueprintCallable)
		virtual void BeginPostPlayPhase(EGameModePlayResult PlayResultValue);

protected:
	EGameModePhase CurrentPhase = EGameModePhase::None;

	// This event allows child blueprints to perform actions when the PrePlayPhase begins.
	// If you override this, you must call BeginPlayPhase() when you are done doing things in the PrePlayPhase.
	// Otherwise the game will be stuck in PrePlayPhase forever.
	// If you don't override this, the game will automatically move on to PlayPhase.
	UFUNCTION(BlueprintNativeEvent)
		void PrePlayPhaseBegan();

	virtual void PrePlayPhaseBegan_Implementation();

	UFUNCTION(BlueprintImplementableEvent)
		void PlayPhaseBegan();

	UFUNCTION(BlueprintImplementableEvent)
		void PostPlayPhaseBegan();
};
