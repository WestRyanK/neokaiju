// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"
#include "TempBlock.generated.h"

class UBreakableStructureAttributeSet;

UCLASS()
class NEOKAIJU_API ATempBlock : public AActor, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATempBlock();

	UPROPERTY(BlueprintReadOnly)
		class UAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY()
		class UBreakableStructureAttributeSet* AttributeSet;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		class TSubclassOf<class UGameplayEffect> InitialAttributeValues;

	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	virtual void Tick(float DeltaTime);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void ListenForAttributeChanges();

	virtual void HealthChanged(const FOnAttributeChangeData& Data);
};
