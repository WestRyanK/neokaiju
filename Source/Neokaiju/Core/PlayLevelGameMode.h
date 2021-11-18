// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Neokaiju/Core/PhasedGameMode.h"
#include "GameplayEffect.h"
#include "PlayLevelGameMode.generated.h"

class UBaseCountdownTimer;
class UUserWidget;
/**
 *
 */
UCLASS()
class NEOKAIJU_API APlayLevelGameMode : public APhasedGameMode
{
	GENERATED_BODY()

public:
	APlayLevelGameMode();

	// The widget that will be displayed before the game starts.
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TSubclassOf<UUserWidget> PreGameMenu;

	// The widget that will be displayed when the game is paused.
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TSubclassOf<UUserWidget> PauseMenu;

	// The widget that will be displayed when the game transitions to PostPlayPhase.
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TSubclassOf<UUserWidget> PostGameMenu;

	// The class that will be used for a countdown timer which transitions to PostPlayPhase.
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TSubclassOf<UBaseCountdownTimer> CountdownTimerClass;
	
	// The actual instance of CountdownTimer used to transition to PostPlayPhase
	// when the Countdown ends.
	UPROPERTY(BlueprintReadOnly)
		UBaseCountdownTimer* CountdownTimer;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TSubclassOf<UGameplayEffect> DisableCharacterInputEffectClass;

	// When there is a request to transition to PostPlayPhase, the game will wait
	// this many seconds before actually transitioning to the PostPlayPhase.
	// This allows the game to play death animations or similar things before
	// pausing the game and displaying the PostGameMenu;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		float PostPlayPhaseTransitionDelay = 3.0f;

	virtual void BeginPlay();

	virtual void EndPlay(const EEndPlayReason::Type Reason);

	UFUNCTION(BlueprintCallable)
		void PauseGame();

	UFUNCTION(BlueprintCallable)
		void UnpauseGame();

	virtual void BeginPrePlayPhase();

	virtual void BeginPlayPhase();

	virtual void BeginPostPlayPhase(EGameModePlayResult PlayResultValue);

	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName="StartMusicAndTimer"))
		void StartMusicAndTimer();

	UPROPERTY(BlueprintReadOnly)
		UUserWidget* SpawnedPreGameMenu;

	UPROPERTY(BlueprintReadOnly)
		UUserWidget* SpawnedPauseMenu;

	UPROPERTY(BlueprintReadOnly)
		UUserWidget* SpawnedPostGameMenu;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		bool bCanPostPlayAffectScore;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		bool bEnableUI = true;

	UFUNCTION(BlueprintPure)
		bool CanScoreBeChanged() const;

protected:
	UFUNCTION()
		void DisableCharacterInput();

	UFUNCTION()
		void EnableCharacterInput();

	UFUNCTION()
		void KaijuDied();

	UFUNCTION()
		void PauseUnpauseRequested();

	UFUNCTION()
		void DisplayPostPlayMenu();

	virtual void PrePlayPhaseBegan_Implementation();

	FActiveGameplayEffectHandle DisableCharacterInputEffectHandle;

};

