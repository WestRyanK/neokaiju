// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Neokaiju/Achievements/TagWatchers/BaseTagWatcher.h"
#include "DestroyEnemyTW.generated.h"

class AActor;
class UWorld;
class USubsystem;
/**
 * 
 */
UCLASS()
class NEOKAIJU_API UDestroyEnemyTW : public UBaseTagWatcher
{
	GENERATED_BODY()
	
public:
	virtual void Init(UWorld* WorldValue);

	virtual void GetSubsystemDependencies(TSet<TSubclassOf<USubsystem>>& OutDependencies) const;
	

protected:
	UFUNCTION()
		void EnemyDestroyed(AActor* DestroyedEnemy);
	
};
