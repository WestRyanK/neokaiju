// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/World.h"
#include "CollisionQueryParams.h"
#include "TraceUtility.generated.h"

/**
 *
 */
UCLASS()
class NEOKAIJU_API UTraceUtility : public UObject
{
	GENERATED_BODY()

public:
	//UFUNCTION(BlueprintCallable)
		static bool BoxTrace(
			const UObject* WorldContextObject,
			struct FHitResult& OutHit,
			const FVector& Start,
			const FVector& End,
			float Width,
			float Depth,
			ECollisionChannel CollisionChannel,
			FCollisionQueryParams QueryParams,
			bool bShouldDrawDebug = false,
			float DrawPersistTime = 0.01f);

		static bool BoxMultiTrace(
			const UObject* WorldContextObject,
			TArray<struct FHitResult>& OutHit,
			const FVector& Start,
			const FVector& End,
			float Width,
			float Depth,
			ECollisionChannel CollisionChannel,
			FCollisionQueryParams QueryParams,
			bool bShouldDrawDebug = false,
			float DrawPersistTime = 0.01f);

private:
	static void DrawDebugBoxSweep(
		UWorld* World,
		bool bShouldDrawDebug,
		const FVector& Start,
		const FVector& End,
		const FVector& CollisionSize,
		const FQuat& CollisionBoxRotation,
		float DrawPersistTime);
};
