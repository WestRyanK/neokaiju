// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Neokaiju/Ability/Core/Tasks/BaseAbilityTask.h"
#include "Neokaiju/Ability/Core/AbilityInputID.h"
#include "Components/InputComponent.h"
#include "WaitForSpecificInputAbilityTask.generated.h"

/**
 *
 */
UCLASS()
class NEOKAIJU_API UWaitForSpecificInputAbilityTask : public UBaseAbilityTask
{
	GENERATED_BODY()

public:
	UWaitForSpecificInputAbilityTask();

	virtual void Activate() override;

	virtual void OnDestroy(bool bInOwnerFinished) override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		EAbilityInputID WaitForInputID;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool bDoesEndOnRelease;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool bDoesEndOnPress;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool bTestInitialState = true;

	virtual void TickTask(float DeltaTime) override;

protected:
	UFUNCTION()
		virtual void OnReleaseNoticed();

	UFUNCTION()
		virtual void OnPressNoticed();

	FInputActionBinding ReleaseBinding;

	FInputActionBinding PressBinding;


private:
	FDelegateHandle OnReleaseDelegateHandle;

	UFUNCTION()
		void OnActionRelease();

	UFUNCTION()
		void OnActionPress();

	bool bWasReleased;

	bool bWasPressed;
};
