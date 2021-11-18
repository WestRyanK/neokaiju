// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SwarmGrabManager.generated.h"


/**
 * 
 */
UCLASS()
class NEOKAIJU_API USwarmGrabManager : public UObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		int32 numSwarmsGrabbing;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		float capturedPosition;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		float capturedVelocity;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		float capturedAcceleration;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		float capturedTime;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		FVector capturedInitPosition;


protected:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		int32 commandingSwarm;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		TArray<bool> availableNums;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		int32 numAvailable;


public:

	USwarmGrabManager();

	UFUNCTION(BluePrintCallable)
		int32 ObtainGrabNumber();

	UFUNCTION(BlueprintCallable)
		void ReleaseGrabNumber(int32 grabNum);

	UFUNCTION(BlueprintCallable)
		float SetCapturePosition(float newCapturePosition, int32 grabNum);

	UFUNCTION(BlueprintCallable)
		float SetCaptureVelocity(float newCaptureVelocity, int32 grabNum);

	UFUNCTION(BlueprintCallable)
		float SetCaptureAcceleration(float newCaptureAcceleration, int32 grabNum);

	UFUNCTION(BlueprintCallable)
		float SetCaptureTime(float newCaptureTime, int32 grabNum);

	UFUNCTION(BlueprintCallable)
		FVector SetCaptureInitPosition(FVector newCaptureInitPosition, int32 grabNum);

	UFUNCTION(BlueprintCallable)
		void AddDashAcceleration(float increaseAccelerationAmount, int32 grabNum);

};
