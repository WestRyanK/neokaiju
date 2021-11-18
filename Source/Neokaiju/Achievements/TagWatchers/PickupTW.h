// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Neokaiju/Achievements/TagWatchers/BaseTagWatcher.h"
#include "PickupTW.generated.h"

class APickupBase;
class UWorld;
class USubsystem;
/**
 * 
 */
UCLASS()
class NEOKAIJU_API UPickupTW : public UBaseTagWatcher
{
	GENERATED_BODY()

public:
	virtual void Init(UWorld* WorldValue);

	virtual void GetSubsystemDependencies(TSet<TSubclassOf<USubsystem>>& OutDependencies) const;
	

protected:
	UFUNCTION()
		void PickupPickedUp(APickupBase* PickupPickedUp);
};
