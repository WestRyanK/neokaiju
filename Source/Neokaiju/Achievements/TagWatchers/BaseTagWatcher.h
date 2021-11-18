// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BaseTagWatcher.generated.h"

class UWorld;
class USubsystem;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTagSeen, FName, SeenTag);
/**
 * 
 */
UCLASS()
class NEOKAIJU_API UBaseTagWatcher : public UObject
{
	GENERATED_BODY()

public:
	virtual void Init(UWorld* WorldValue);

	virtual void GetSubsystemDependencies(TSet<TSubclassOf<USubsystem>>& OutDependencies) const { };

	UPROPERTY(BlueprintAssignable)
		FOnTagSeen OnTagSeen;

protected:
	UWorld* World;

	void TagSeen(FName SeenTag);
};
