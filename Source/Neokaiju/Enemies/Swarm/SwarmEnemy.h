// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AbilitySystemInterface.h"
#include "Neokaiju/Ability/EnvironmentAbilities/CaptureAbility/CapturerInterface.h"
#include "GameplayTags.h"
#include "SwarmEnemy.generated.h"


class UAbilitySystemComponent;
UCLASS()
class NEOKAIJU_API ASwarmEnemy : public AActor, public IAbilitySystemInterface, public ICapturerInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASwarmEnemy();

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		UAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FGameplayTag HitByLaserTag;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TSubclassOf<class UCaptureAbility> CaptureAbility;

	// Inherited via IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void KillSwarm();

protected:
	virtual void BeginPlay() override;

	void ListenForTags();

	UFUNCTION()
		void HitByLaserTagChanged(FGameplayTag Tag, int32 Count);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void BeginHitByLaser();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void EndHitByLaser();
};
