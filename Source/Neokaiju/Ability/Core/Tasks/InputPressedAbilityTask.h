// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Neokaiju/Ability/Core/Tasks/WaitForSpecificInputAbilityTask.h"
#include "InputPressedAbilityTask.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInputPressedAbilityTaskDelegate);
/**
 *
 */
UCLASS()
class NEOKAIJU_API UInputPressedAbilityTask : public UWaitForSpecificInputAbilityTask
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
		static UInputPressedAbilityTask* InputPressed(
			UGameplayAbility* OwningAbility, 
			EAbilityInputID Input);

	UPROPERTY(BlueprintAssignable)
		FInputPressedAbilityTaskDelegate OnPressed;

protected:
	virtual void OnPressNoticed() override;

};
