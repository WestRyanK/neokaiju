// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Neokaiju/Achievements/TagWatchers/BaseTagWatcher.h"
#include "DestroyStructureTW.generated.h"

class ABreakableStructure;
class UWorld;
class USubsystem;
/**
 * 
 */
UCLASS()
class NEOKAIJU_API UDestroyStructureTW : public UBaseTagWatcher
{
	GENERATED_BODY()

public:
	virtual void Init(UWorld* WorldValue);

	virtual void GetSubsystemDependencies(TSet<TSubclassOf<USubsystem>>& OutDependencies) const;
	

protected:
	UFUNCTION()
		void StructureDestroyed(ABreakableStructure* DestroyedStructure);
	
};
