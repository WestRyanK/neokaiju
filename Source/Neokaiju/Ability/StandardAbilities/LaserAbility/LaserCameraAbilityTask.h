// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "Neokaiju/Ability/Core/Tasks/BaseAbilityTask.h"
#include "LaserCameraAbilityTask.generated.h"

class AKaijuCharacter;
class UCameraShake;
class UCameraComponent;

/**
 * 
 */
UCLASS()
class NEOKAIJU_API ULaserCameraAbilityTask : public UBaseAbilityTask
{
	GENERATED_BODY()
	
public:
	ULaserCameraAbilityTask();

	// Static Constructor for UDashAbilityTask
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
		static ULaserCameraAbilityTask* LaserCamera(
			UGameplayAbility* OwningAbility,
			TSubclassOf<UCameraShake> CameraShake,
			float LaserCameraOffset,
			bool bShouldCenterCameraOnActor);

	virtual void Activate() override;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
		TSubclassOf<UCameraShake> CameraShake;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
		float LaserCameraOffset;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
		bool bShouldCenterCameraOnActor;

	virtual void TickTask(float DeltaTime);

	void FireLaserComplete();

	void SetLaserAimDirection(FVector LaserAimDirectionValue);

	void UpdateCameraPosition(float DeltaTime);

protected:
	virtual void OnDestroy(bool bInOwnerFinished);

private:
	FVector LaserAimDirection;

	USceneComponent* CameraFocus;

	FVector OriginalCameraLocation;

	FVector CameraPivotCenter;
};
