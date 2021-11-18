// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Neokaiju/Pickups/PickupBase.h"
#include "Neokaiju/Environment/BaseTrigger.h"
#include "BabyKaijuPickup.generated.h"

class USphereComponent;
class ASwarmV2;
class ACameraGuideTrigger;
/**
 * 
 */
UCLASS()
class NEOKAIJU_API ABabyKaijuPickup : public APickupBase
{
	GENERATED_BODY()

public:
	ABabyKaijuPickup();

	//UPROPERTY(BlueprintReadOnly, EditAnywhere)
	//	TSubclassOf<AActor> ActorTypeToDestroy;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		USphereComponent* DestroyActorsVolume;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		ABaseTrigger* DestroyTriggerOnPickup;
	
protected:	
	virtual void PickUp(AKaijuCharacter* Character);

	void DestroyActorsInVolume();

	void NotifyBabyKaijuSaved();

	TArray<ASwarmV2*> ActorsToDestroy;
};
