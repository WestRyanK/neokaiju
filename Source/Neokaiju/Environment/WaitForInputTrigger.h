// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Neokaiju/Environment/EasingTrigger.h"
#include "GameplayEffect.h"
#include "WaitForInputTrigger.generated.h"

/**
 *
 */
UCLASS()
class NEOKAIJU_API AWaitForInputTrigger : public AEasingTrigger
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TSubclassOf<UGameplayEffect> DisableCharacterInputEffectClass;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FName WaitForInputName;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		float ReenableCharacterInputAfterWaitForInputPressedDelay;

protected:
	virtual void OnBeginTrigger(AActor* TriggeringActorValue);

	void EnableCharacterInput();

	UFUNCTION()
		virtual void WaitForInputPressed();

	AActor* TriggeringActor;

	UAbilitySystemComponent* TriggeringASC;

	FActiveGameplayEffectHandle DisableCharacterInputEffectHandle;

	FInputActionBinding WaitForInputBinding;
};
