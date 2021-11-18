// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Neokaiju/Environment/BaseTrigger.h"
#include "Neokaiju/Core/PhasedGameMode.h"
#include "GamePhaseTrigger.generated.h"

/**
 *
 */
UCLASS()
class NEOKAIJU_API AGamePhaseTrigger : public ABaseTrigger
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Trigger")
		EGameModePhase PhaseToTrigger;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Trigger")
		EGameModePlayResult PlayResultToTrigger;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Trigger")
		bool bShouldChangePhaseOnEnter = true;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Trigger")
		bool bShouldChangePhaseOnExit = false;

protected:

	UFUNCTION()
		void ChangePhase();

	virtual void OnBeginTrigger(AActor* TriggeringActor);

	virtual void OnEndTrigger(AActor* TriggeringActor);

};
