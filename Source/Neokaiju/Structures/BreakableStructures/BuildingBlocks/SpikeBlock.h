// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Neokaiju/Structures/BreakableStructures/BuildingBlocks/BuildingBlock.h"
#include "SpikeBlock.generated.h"

class AKaijuCharacter;

/**
 * 
 */
UCLASS()
class NEOKAIJU_API ASpikeBlock : public ABuildingBlock
{
	GENERATED_BODY()
	
public:
	ASpikeBlock();

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TSubclassOf<class UGameplayEffect> SpikeDamageEffect;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		float MaxLevelKaijuToEffect;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Component")
		class UBoxComponent* SpikeOverlapBox;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Component")
		FTimerHandle timerHandle;



protected:

	virtual void BeginPlay() override;

	UFUNCTION()
		void OnHit(
			UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			FVector NormalImpulse,
			const FHitResult& Hit);

	UFUNCTION()
		void CheckForOverlaps();
};
