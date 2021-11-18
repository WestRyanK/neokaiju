// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "EnemySubsystem.generated.h"



DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnemyDestroyed, AActor*, DestroyedEnemy);

class USwarmGrabManager;

/**
 * 
 */
UCLASS()
class NEOKAIJU_API UEnemySubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection);

	UPROPERTY(BlueprintReadOnly)
		USwarmGrabManager* SwarmGrabManager;

	UPROPERTY(BlueprintAssignable)
		FOnEnemyDestroyed OnEnemyDestroyed;

	UFUNCTION(BlueprintCallable)
		void EnemyDestroyed(AActor* DestroyedEnemy);
};
