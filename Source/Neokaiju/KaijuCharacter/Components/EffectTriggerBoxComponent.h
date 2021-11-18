// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameplayEffect.h"
#include "EffectTriggerBoxComponent.generated.h"

/**
 *
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class NEOKAIJU_API UEffectTriggerBoxComponent : public UBoxComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TSubclassOf<UGameplayEffect> TriggeredEffect;

	virtual void BeginPlay() override;

	virtual void OnComponentDestroyed(bool bDestroyingHierarchy) override;

protected:
	UFUNCTION()
		void BeginOverlapTrigger(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void EndOverlapTrigger(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	FActiveGameplayEffectHandle ActiveEffectHandle;

	UAbilitySystemComponent* GetOwnerASC() const;

};
