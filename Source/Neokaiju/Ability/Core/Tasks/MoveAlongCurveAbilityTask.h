// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Neokaiju/Ability/Core/Tasks/BaseAbilityTask.h"
#include "MoveAlongCurveAbilityTask.generated.h"

class UCurveFloat;
class ACharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMoveAlongCurveAbilityTaskCompletedDelegate);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FMoveAlongCurveAbilityTaskDelegate, FVector, CharacterVelocity, FVector, ImpactVelocity, AActor*, HitActor);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FMoveAlongCurveAbilityTaskTickDelegate, float, DeltaTime, FVector, MoveAlongCurveInDirection);
/**
 *
 */
UCLASS()
class NEOKAIJU_API UMoveAlongCurveAbilityTask : public UBaseAbilityTask
{
	GENERATED_BODY()

public:

	UMoveAlongCurveAbilityTask();

	UPROPERTY()
		FVector MoveAlongCurveInDirection;

	UPROPERTY()
		float MoveAlongCurveOfLength;

	UPROPERTY()
		float MoveAlongCurveForDuration;

	UPROPERTY()
		UCurveFloat* PositionOverTime;

	UPROPERTY()
		bool bIsMovementAbsoluteAlongCurve;

	UPROPERTY(BlueprintAssignable)
		FMoveAlongCurveAbilityTaskTickDelegate OnTick;

	UPROPERTY(BlueprintAssignable)
		FMoveAlongCurveAbilityTaskCompletedDelegate OnCompleted;

	virtual void Activate() override;

	virtual void TickTask(float DeltaTime) override;

	virtual void Finish();

	virtual void OnDestroy(bool bInOwnerFinished);

protected:
	UFUNCTION()
		virtual void OnHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit);

	float GetProgressAtPercent(float PercentComplete) const;

	float GetDerivativeAtPercent(float PercentComplete, float Delta = DELTA) const;

	float GetPercentComplete();

	FVector LocationAtLastDirectionChange;

	float ProgressAtLastDirectionChange;

	float StartTime;

	float EndTime;

	ACharacter* Character;
};
