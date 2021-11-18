// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SwarmStats.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class NEOKAIJU_API USwarmStats : public UDataAsset
{
	GENERATED_BODY()
	
public:

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Movement")
		float ChaseSpeed;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Movement")
		float StrafeSpeed;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Movement")
		float TransformSpeed;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Movement")
		float AttackSpeed;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Rotation")
		float ChaseRotationSpeed;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Rotation")
		float StrafeRotationSpeed;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Rotation")
		float AimRotationSpeed;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Detection")
		float IdleOuterRadius;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Detection")
		float ChaseBeginCircleRadius;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Detection")
		float ChaseEndPursuitRadius;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Detection")
		float StrafeOuterRadius;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Timing")
		float StrafeTime;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Timing")
		float TransformTime;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Timing")
		float AimTime;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Timing")
		float MaxAttackTime;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Timing")
		float StuckTime;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Timing")
		float ScatterDisperseTime;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Timing")
		float ScatterAssembleTime;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Timing")
		float ScatterTime;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Scatter")
		float ScatterDisperseY;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Scatter")
		float ScatterDisperseZ;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Grab")
	    int32 DashesToEscapeGrab;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Grab", meta=(ClampMin="1.0"))
		float GrabKValue;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Grab")
		float GrabVelocityMultiplier;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Timing")
		float ReverseTime;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Movement")
		float ReverseSpeed;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Timing")
		float PreAttackReverseTime;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Movement")
		float PreAttackReverseSpeed;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Timing")
		float TimeForLazerToKill;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Spin")
		float LazerReactionSpin;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Spin")
		float NormalSpin;


};
