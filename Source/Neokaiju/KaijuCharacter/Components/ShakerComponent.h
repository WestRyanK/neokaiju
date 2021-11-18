// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Camera/CameraShake.h"
#include "TimerManager.h"
#include "ShakerComponent.generated.h"


UCLASS(Blueprintable)
class NEOKAIJU_API UShake : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		bool bReturnsToOriginalTransform = false;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		float Duration = 1.0f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FVOscillator PositionOscillator;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FROscillator RotationOscillator;
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class NEOKAIJU_API UShakerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UShakerComponent();

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TSubclassOf<UShake> ShakeClass;

	UFUNCTION(BlueprintCallable)
		void PlayShakeOnActor();

	UFUNCTION(BlueprintCallable)
		void EndShake();

	UFUNCTION(BlueprintCallable, CallInEditor)
		void DemoShakeInEditor();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	bool bIsCurrentlyShaking = false;

	FVector OriginalPosition;

	FRotator OriginalRotation;

	float ShakeTimeElapsed = 0.0f;

	UPROPERTY(BlueprintReadOnly)
		UShake* Shake;

	FVector CurrentPositionTimeOffset;

	FVector CurrentRotationTimeOffset;

	UFUNCTION()
		void ShakeTick(float DeltaTime);

	void SetTickEnabled(bool TickEnabled);

private:
	bool bIsEditorDemo = false;

};
