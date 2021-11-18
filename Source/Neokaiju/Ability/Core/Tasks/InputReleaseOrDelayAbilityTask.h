// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Neokaiju/Ability/Core/Tasks/WaitForSpecificInputAbilityTask.h"
#include "InputReleaseOrDelayAbilityTask.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInputReleaseOrDelayAbilityTaskDelegate, float, TimeRemaining);
/**
 *
 */
UCLASS()
class NEOKAIJU_API UInputReleaseOrDelayAbilityTask : public UWaitForSpecificInputAbilityTask
{
	GENERATED_BODY()

public:
	UInputReleaseOrDelayAbilityTask();

	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
		static UInputReleaseOrDelayAbilityTask* WaitInputReleaseOrDelay(
			UGameplayAbility* OwningAbility,
			float DelayTimeSeconds);

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
		float DelayTimeSeconds;

	UPROPERTY(BlueprintAssignable)
		FInputReleaseOrDelayAbilityTaskDelegate OnTick;

	UPROPERTY(BlueprintAssignable)
		FInputReleaseOrDelayAbilityTaskDelegate OnComplete;

	UPROPERTY(BlueprintAssignable)
		FInputReleaseOrDelayAbilityTaskDelegate OnCancel;

	virtual void Activate() override;

	virtual void TickTask(float DeltaTime) override;

protected:

	virtual void OnReleaseNoticed();

	float StartTimeSeconds;

	float GetTimeRemaining() const;
};
