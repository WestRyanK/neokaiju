// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameFramework/Actor.h"
#include "SafeDirectionUtility.generated.h"

UENUM(BlueprintType)
enum class EFacingDirection : uint8
{
	NONE = 0 UMETA(Display = "None"),
	RIGHT = 1 UMETA(DisplayName = "Right"),
	LEFT = 2 UMETA(DisplayName = "Left")
};
/**
 *
 */
UCLASS()
class NEOKAIJU_API USafeDirectionUtility : public UObject
{
	GENERATED_BODY()

public:
	// Rotates InputVector in the direction pointed to by TowardsDirection by rotating around the Y axis, taking the shortest path.
	// User can specify which direction to turn if InputVector and TowardsDirection are 180 degrees from each other.
	// The amount that InputVector is rotated is at most RotateSpeed * DeltaTime. If that value is greater than the angular distance,
	// InputVector is simply set to face the direction of TowardsDirection.
	// If RotateSpeed <= 0, the rotation is instant.
	// If the InputVector is 180 degrees from TowardsDirection and TurnDirectionIf(Non)Vertical180 is ZeroVector, then rotation is instant.
	UFUNCTION(BlueprintCallable, BlueprintPure)
		static FVector RotateVectorOnYAxisTowards(
			FVector InputVector,
			FVector TowardsDirection,
			FVector TurnDirectionIfVertical180 = FVector::ZeroVector,
			FVector TurnDirectionIfNonVertical180 = FVector::ZeroVector,
			float RotateSpeed = 0.0f,
			float DeltaTime = 0.0f);

	// Determines which direction - whether clockwise (OutDirection = -1) or counterclockwise (OutDirection = 1) - to turn
	// InputVector so it faces TowardsDirection. User can specify which direction to turn in 180 degree situations.
	// Returns true if DirectionChange could be determined and false if it couldn't.
	// DirectionChange can be determined when DeltaAngle < 180, or when DeltaAngle == 180 and TurnDirectionIf(Non)Vertical180
	// is not ZeroVector. If it is ZeroVector, then the direction to change cannot be determined.
	UFUNCTION(BlueprintCallable, BlueprintPure)
		static bool GetDirectionChange(
			FVector InputVector,
			FVector TowardsDirection,
			FVector TurnDirectionIfVertical180,
			FVector TurnDirectionIfNonVertical180,
			float& OutDirectionChange,
			float& OutDeltaAngle);

	// Returns either (0, 1, 0) or (0, -1, 0) depending on whether the Actor
	// is pointed right or left.
	// It is important to use this function on the result of Actor->GetActorRightVector()
	// in order to prevent drift away from exact directions which can occur occasionally,
	// and is undesirable in this 2d game.
	UFUNCTION(BlueprintCallable, BlueprintPure)
		static FVector GetSafeRightVector(FVector UnsafeRightVector);

	// Returns either (1, 0, 0) or (-1, 0, 0) depending on whether the Actor is facing right
	// or left. If the actor is facing straight up or straight down, then the ForwardDirection
	// is determined as if the Actor were rotated so the bottom is level with the ground again.
	UFUNCTION(BlueprintCallable, BlueprintPure)
		static FVector GetSafeForwardDirectionFromRightVector(FVector UnsafeRightVector);

	// Returns either (1, 0, 0) or (-1, 0, 0) depending on whether the Actor is facing right
	// or left. If the actor is facing straight up or straight down, then the ForwardDirection
	// is determined as if the Actor were rotated so the bottom is level with the ground again.
	UFUNCTION(BlueprintCallable, BlueprintPure)
		static FVector GetSafeForwardDirectionFromFacing(EFacingDirection FacingDirection);

	// Given an X Axis value, get whether it is pointing left or right.
	UFUNCTION(BlueprintCallable, BlueprintPure)
		static EFacingDirection GetFacingDirectionFromAxisValue(float AxisValue);


	// Returns a Rotator for the Actor being rotated in the direction pointed to by DesiredDirection by rotating around the Y axis, taking the shortest path.
	// User can specify which direction to turn if the Actor is facing 180 degrees away from TowardsDirection.
	// The amount of the Actor's Rotation is at most RotateSpeed * DeltaTime. If that value is greater than the angular distance,
	// The Rotator simply faces DesiredDirection.
	// If RotateSpeed <= 0, the rotation is instant.
	// If the Actor is 180 degrees from TowardsDirection and TurnDirectionIf(Non)Vertical180 is ZeroVector, then rotation is instant.
	//
	// If bForceUseSafeRightVector is false, everything above holds true, otherwise the following happens:
	// Instead of trying to maintain +Z of the rotator pointing up, flipping +Y accordingly, +Y will be forced to be in
	// the direction of UnsafeRightVector, allowing for +Z to point down sometimes.
	UFUNCTION(BlueprintCallable, BlueprintPure)
		static FRotator GetSafePitchRotation(
			FVector UnsafeRightVector,
			FVector CurrentForwardVector,
			FVector DesiredDirection,
			FVector TurnDirectionIfVertical180 = FVector::ZeroVector,
			FVector TurnDirectionIfNonVertical180 = FVector::ZeroVector,
			float RotateSpeed = 0.0f,
			float DeltaTime = 0.0f,
			bool bForceUseSafeRightVector = false);

	UFUNCTION(BlueprintCallable, BlueprintPure)
		static float GetAngleOfVectorAroundYAxis(FVector Vector);

	UFUNCTION(BlueprintCallable, BlueprintPure)
		static float GetAngleOfVectorAwayFromDirection(FVector Vector, EFacingDirection Direction);
};
