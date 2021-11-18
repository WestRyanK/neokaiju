// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Neokaiju/Environment/EasingTrigger.h"
#include "TimerManager.h"
#include "CameraViewTrigger.generated.h"

class USceneComponent;
class UCameraComponent;
/**
 *
 */
UCLASS()
class NEOKAIJU_API ACameraViewTrigger : public AEasingTrigger
{
	GENERATED_BODY()

public:
	ACameraViewTrigger();

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		UCameraComponent* CameraView;

protected:
	virtual void OnBeginTrigger(AActor* TriggeringActor);

	virtual void OnEndTrigger(AActor* TriggeringActor);

	FTimerHandle TransitioningTimerHandle;

	UFUNCTION()
		void OnTransitionCompleted();
};
