// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Neokaiju/Environment/BaseTrigger.h"
#include "CameraGuideTrigger.generated.h"

class USplineComponent;
//class UCameraComponent;
class USceneComponent;
/**
 *
 */
UCLASS()
class NEOKAIJU_API ACameraGuideTrigger : public ABaseTrigger
{
	GENERATED_BODY()

public:
	ACameraGuideTrigger();

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		USplineComponent* CameraPath;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Trigger")
		FString TagOfComponentToGuide;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Trigger")
		bool bIgnoreYAxis = false;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Trigger")
		float InterpSpeed = 10.0f;

	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY()
		USceneComponent* GuidedActorComponent;

	UPROPERTY()
		FVector OriginalGuidedActorComponentLocation;

	bool bIsTransitioningIn = false;

	bool bIsTransitioningOut = false;

	FVector GetGuideLocation();

	virtual void OnBeginTrigger(AActor* TriggeringActor);

	virtual void OnEndTrigger(AActor* TriggeringActor);
};
