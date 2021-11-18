// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TimerManager.h"
#include "LerpUtility.generated.h"

UENUM()
enum class ELerpSetValue : uint8
{
	CurrentValue,
	StartValue,
	EndValue
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FLerpUtilityVectorUpdated, FVector, UpdatedValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FLerpUtilityFloatUpdated, float, UpdatedValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FLerpUtilityCompleted);

/**
 *
 */
UCLASS(Blueprintable)
class NEOKAIJU_API ULerpUtility : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
		static ULerpUtility* LerpVectorOverTime(UObject* WorldContextObject, UObject* Outer, FVector Start, FVector End, float Duration, float TickInterval, float InitialDelay = 0.0f);

	UFUNCTION(BlueprintCallable)
		static ULerpUtility* LerpFloatOverTime(UObject* WorldContextObject, UObject* Outer, float Start, float End, float Duration, float TickInterval, float InitialDelay = 0.0f);

	UFUNCTION(BlueprintCallable)
		void Begin();

	UFUNCTION(BlueprintCallable)
		void End(ELerpSetValue SetValue);

	UPROPERTY(BlueprintAssignable)
		FLerpUtilityVectorUpdated VectorUpdated;
	
	UPROPERTY(BlueprintAssignable)
		FLerpUtilityFloatUpdated FloatUpdated;

	UPROPERTY(BlueprintAssignable)
		FLerpUtilityCompleted Completed;

protected:
	UFUNCTION()
		void LerpUpdate();

	UFUNCTION()
		void BeginLerpTransition();

	static ULerpUtility* CreateLerpUtility(UObject* WorldContextObject, UObject* Outer, float Duration, float TickInterval, float InitialDelay);

	UObject* WorldContextObject;

	float StartFloat;

	float EndFloat;

	float CurrentFloat;

	FVector StartVector;

	FVector EndVector;

	FVector CurrentVector;

	float Alpha;

	float AlphaIncrement;

	float InitialDelay;

	float Duration;

	float TickInterval;

	FTimerHandle TimerHandle;

	bool bIsTransitioning = false;
};
