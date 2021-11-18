// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StreamingStateManager.generated.h"

UCLASS()
class NEOKAIJU_API AStreamingStateManager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AStreamingStateManager();

	UFUNCTION(BlueprintCallable)
		void SaveLevelState();

	UFUNCTION(BlueprintCallable)
		void LoadLevelState();

protected:
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

};
