// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Neokaiju/Pickups/PickupBase.h"
#include "PickupSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class NEOKAIJU_API UPickupSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:

	UPROPERTY(BlueprintAssignable)
		FOnPickupPickedUp OnPickupPickedUp;

	UFUNCTION(BlueprintCallable)
		void PickupPickedUp(APickupBase* PickupPickedUp);
};
