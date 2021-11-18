// Fill out your copyright notice in the Description page of Project Settings.


#include "TraceUtility.h"
#include "Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"

bool UTraceUtility::BoxTrace(
	const UObject* WorldContextObject,
	struct FHitResult& OutHit,
	const FVector& Start,
	const FVector& End,
	float Width,
	float Depth,
	ECollisionChannel CollisionChannel,
	FCollisionQueryParams QueryParams,
	bool bShouldDrawDebug,
	float DrawPersistTime)
{
	if (IsValid(WorldContextObject))
	{
		UWorld* World = WorldContextObject->GetWorld();
		if (IsValid(World))
		{
			FVector CollisionSize = FVector(1.0f, Depth, Width);
			FQuat CollisionBoxRotation = FQuat(UKismetMathLibrary::FindLookAtRotation(Start, End));

			UTraceUtility::DrawDebugBoxSweep(World, bShouldDrawDebug, Start, End, CollisionSize, CollisionBoxRotation, DrawPersistTime);

			bool bHitOccurred = World->SweepSingleByChannel(
				OutHit,
				Start,
				End,
				CollisionBoxRotation,
				CollisionChannel,
				FCollisionShape::MakeBox(CollisionSize),
				QueryParams);
			return bHitOccurred;
		}
	}
	return false;
}

bool UTraceUtility::BoxMultiTrace(
	const UObject* WorldContextObject,
	TArray<struct FHitResult>& OutHit,
	const FVector& Start,
	const FVector& End,
	float Width,
	float Depth,
	ECollisionChannel CollisionChannel,
	FCollisionQueryParams QueryParams,
	bool bShouldDrawDebug,
	float DrawPersistTime) {

	if (IsValid(WorldContextObject))
	{
		UWorld* World = WorldContextObject->GetWorld();
		if (IsValid(World))
		{
			FVector CollisionSize = FVector(1.0f, Depth, Width);
			FQuat CollisionBoxRotation = FQuat(UKismetMathLibrary::FindLookAtRotation(Start, End));

			UTraceUtility::DrawDebugBoxSweep(World, bShouldDrawDebug, Start, End, CollisionSize, CollisionBoxRotation, DrawPersistTime);

			bool bHitOccurred = World->SweepMultiByChannel(
				OutHit,
				Start,
				End,
				CollisionBoxRotation,
				CollisionChannel,
				FCollisionShape::MakeBox(CollisionSize),
				QueryParams);
			return bHitOccurred;
		}
	}
	return false;
	
}

void UTraceUtility::DrawDebugBoxSweep(
	UWorld* World,
	bool bShouldDrawDebug,
	const FVector& Start,
	const FVector& End,
	const FVector& CollisionSize,
	const FQuat& CollisionBoxRotation,
	float DrawPersistTime)
{
#if !UE_BUILD_SHIPPING // Don't draw trace when shipping
	if (bShouldDrawDebug)
	{
		float Distance = FVector::Distance(Start, End) * 0.5f;
		FVector CenterPoint = (Start + End) * 0.5f;
		DrawDebugBox(World, CenterPoint, CollisionSize * FVector(Distance, 1.0f, 1.0f), CollisionBoxRotation, FColor::Red, false, DrawPersistTime, 0, 2.0f);
	}
#endif
}
