// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Neokaiju/Environment/EasingTrigger.h"
#include "SequenceTrigger.generated.h"

class USeamlessSequencePlayer;
class ALevelSequenceActor;
class AActor;
class ACameraActor;
/**
 *
 */
UCLASS()
class NEOKAIJU_API ASequenceTrigger : public AEasingTrigger
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Trigger")
		ALevelSequenceActor* TriggeredSequence;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Trigger")
		ACameraActor* TrigeredSequenceCamera;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Trigger")
		bool bIsInputDisabledDuringSequence;

	//UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Trigger")
	//	bool bShouldEndSequenceOnExit;

protected:
	virtual void OnBeginTrigger(AActor* TriggeringActor);

	virtual void OnEndTrigger(AActor* TriggeringActor);

	UFUNCTION()
		void OnSequenceFinished();

	UPROPERTY()
		USeamlessSequencePlayer* SequencePlayer;

};
