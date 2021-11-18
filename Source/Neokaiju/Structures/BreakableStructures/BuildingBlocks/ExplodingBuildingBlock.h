// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Neokaiju/Structures/BreakableStructures/BuildingBlocks/BuildingBlock.h"
#include "TimerManager.h"
#include "ExplodingBuildingBlock.generated.h"

/**
 *
 */
UCLASS()
class NEOKAIJU_API AExplodingBuildingBlock : public ABuildingBlock
{
	GENERATED_BODY()

public:
	virtual void DestroyStructure() override;

	UFUNCTION(BlueprintCallable)
		void Explode();

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TMap<float, TSubclassOf<UGameplayEffect>> ExplodeEffectsAtRadii;

	// The level passed to the GameplayEffects which are applied to hit Actors.
	// The GameplayEffects can then choose to use the value or not. If 
	// bIsExplodeLevelBasedOnProportionOfRadius = true, then this value is a 
	// multiplier of the proportion of distance from epicenter to radius.
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		float ExplodeLevel = 1.0f;

	// If this is true, the ExplodeLevel (which can be used by the GameplayEffect
	// to change how much damage is done) is relative to the distance of the hit object
	// from the epicenter based on the max explode distance.
	// So if an object is hit 30m from the epicenter with an explode radius of 40m and
	// and ExplodeLevel of 2.0f, the actual value will be 30.0f / 40.0f * 2.0f = 1.5f
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		bool bIsExplodeLevelBasedOnProportionOfRadius;

	// The block will explode between ExplodeDelayMinSeconds and ExplodeDelayMaxSeconds
	// after reaching 0 health.
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		float ExplodeDelayMinSeconds = 0.4f;

	// The block will explode between ExplodeDelayMinSeconds and ExplodeDelayMaxSeconds
	// after reaching 0 health.
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		float ExplodeDelayMaxSeconds = 0.6f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		bool bDrawDebugExplosions;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void OnExplode(FVector Epicenter);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void OnAboutToExplode(float TimeUntilExplosion);

	UFUNCTION(BlueprintCallable, BlueprintPure)
		float GetMaxExplodeRadius() const;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FGameplayTag ThrownTag;

	virtual void HealthChanged(const FOnAttributeChangeData& Data);

protected:
		virtual void OnMeshHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;

private:
	FTimerHandle ExplodeTimerHandle;

	bool bHasAlreadyExploded = false;

	bool bHasBeenDamaged = false;
};
