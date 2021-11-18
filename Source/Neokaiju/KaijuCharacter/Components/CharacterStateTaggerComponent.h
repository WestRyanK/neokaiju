// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffect.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CharacterStateTaggerComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class NEOKAIJU_API UCharacterStateTaggerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCharacterStateTaggerComponent();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TMap<TEnumAsByte<EMovementMode>, TSubclassOf<UGameplayEffect>> MovementModeEffects;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		float UpdateTagTick = 0.1f;

	//virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	TMultiMap<TEnumAsByte<EMovementMode>, FActiveGameplayEffectHandle> MovementModeEffectHandles;

	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void DestroyComponent(bool bPromoteChildren = false) override;

	ACharacter* OwnerCharacter;

	UAbilitySystemComponent* OwnerASC;

	UFUNCTION()
		void OnMovementModeChanged(ACharacter* Character, EMovementMode PrevMovementMode, uint8 PreviousCustomMode);
};
