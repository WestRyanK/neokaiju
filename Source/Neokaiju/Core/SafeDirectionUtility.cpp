// Fill out your copyright notice in the Description page of Project Settings.


#include "SafeDirectionUtility.h"
#include "Kismet/KismetMathLibrary.h"


FVector USafeDirectionUtility::RotateVectorOnYAxisTowards(FVector InputVector, FVector TowardsDirection, FVector TurnDirectionIfVertical180, FVector TurnDirectionIfNonVertical180, float RotateSpeed, float DeltaTime)
{
	float DeltaAngle;
	float DirectionChange;
	bool bIsInstantRotation = !USafeDirectionUtility::GetDirectionChange(InputVector, TowardsDirection, TurnDirectionIfVertical180, TurnDirectionIfNonVertical180, DirectionChange, DeltaAngle);

	float RotateAngle = DeltaTime * RotateSpeed * DirectionChange;

	FVector OutputVector;
	if (RotateSpeed > 0 && FMath::Abs(RotateAngle) < FMath::Abs(DeltaAngle) && !bIsInstantRotation)
	{
		OutputVector = InputVector.RotateAngleAxis(-RotateAngle, FVector::RightVector);
	}
	else
	{
		OutputVector = TowardsDirection;
	}

	OutputVector.Normalize();
	return OutputVector;
}

float USafeDirectionUtility::GetAngleOfVectorAroundYAxis(FVector Vector)
{
	float VectorAngle = FMath::RadiansToDegrees(FMath::Atan2(Vector.Z, Vector.X));
	return VectorAngle;
}

float USafeDirectionUtility::GetAngleOfVectorAwayFromDirection(FVector Vector, EFacingDirection Direction)
{
	if (Direction == EFacingDirection::LEFT)
	{
		Vector.X = -Vector.X;
	}
	float Angle = USafeDirectionUtility::GetAngleOfVectorAroundYAxis(Vector);
	return Angle;
}

bool USafeDirectionUtility::GetDirectionChange(FVector InputVector, FVector TowardsDirection, FVector TurnDirectionIfVertical180, FVector TurnDirectionIfNonVertical180, float& OutDirectionChange, float& OutDeltaAngle)
{
	float InputVectorAngle = USafeDirectionUtility::GetAngleOfVectorAroundYAxis(InputVector);
	float TowardsDirectionAngle = USafeDirectionUtility::GetAngleOfVectorAroundYAxis(TowardsDirection);
	OutDeltaAngle = FMath::FindDeltaAngleDegrees(InputVectorAngle, TowardsDirectionAngle);

	// It is obvious which way to turn in this case, just turn in the shortest direction.
	if (FMath::Abs(OutDeltaAngle) < 180.0f)
	{
		OutDirectionChange = FMath::Sign(OutDeltaAngle);
	}
	else
	{
		float AbsInputVectorAngle = FMath::Abs(InputVectorAngle);
		FVector TurnDirection180;
		if (AbsInputVectorAngle == 90.0f || AbsInputVectorAngle == 270.0f) // If vertical
		{
			TurnDirection180 = TurnDirectionIfVertical180;
		}
		else
		{
			TurnDirectionIfNonVertical180;
		}
		if (TurnDirection180 == FVector::ZeroVector)
		{
			OutDirectionChange = 0.0f;
			return false;
		}

		float RotateTowardsAngle = FMath::RadiansToDegrees(FMath::Atan2(TurnDirection180.Z, TurnDirection180.X));

		OutDirectionChange = FMath::Sign(FMath::FindDeltaAngleDegrees(InputVectorAngle, RotateTowardsAngle));
	}
	return true;
}

FRotator USafeDirectionUtility::GetSafePitchRotation(
	FVector UnsafeRightVector,
	FVector CurrentForwardVector,
	FVector DesiredDirection,
	FVector TurnDirectionIfVertical180,
	FVector TurnDirectionIfNonVertical180,
	float RotateSpeed,
	float DeltaTime,
	bool bForceUseSafeRightVector)
{
	FVector RightVector = USafeDirectionUtility::GetSafeRightVector(UnsafeRightVector);

	if (!bForceUseSafeRightVector)
	{
		if (DesiredDirection.X < 0)
		{
			RightVector = FVector::LeftVector;
		}
		else if (DesiredDirection.X > 0)
		{
			RightVector = FVector::RightVector;
		}
	}

	FVector UpdatedForward = USafeDirectionUtility::RotateVectorOnYAxisTowards(
		CurrentForwardVector,
		DesiredDirection,
		TurnDirectionIfVertical180,
		TurnDirectionIfNonVertical180,
		RotateSpeed,
		DeltaTime);

	//bOutDidFlipLeftRight = FMath::Sign(CurrentForwardVector.X) != FMath::Sign(UpdatedForward.X);

	FRotator ActorRotation = UKismetMathLibrary::MakeRotFromXY(UpdatedForward, RightVector);
	return ActorRotation;
}

FVector USafeDirectionUtility::GetSafeRightVector(FVector UnsafeRightVector)
{
	if (UnsafeRightVector.Y > 0)
	{
		return FVector::RightVector;
	}
	else
	{
		return FVector::LeftVector;
	}
}

FVector USafeDirectionUtility::GetSafeForwardDirectionFromRightVector(FVector UnsafeRightVector)
{
	FVector SafeRightVector = USafeDirectionUtility::GetSafeRightVector(UnsafeRightVector);
	FVector SafeForwardDirection = SafeRightVector ^ FVector::UpVector;
	return SafeForwardDirection;
}

FVector USafeDirectionUtility::GetSafeForwardDirectionFromFacing(EFacingDirection FacingDirection)
{
	switch (FacingDirection)
	{
	case EFacingDirection::RIGHT:
		return FVector::ForwardVector;
	case EFacingDirection::LEFT:
		return FVector::BackwardVector;
	default:
		return FVector::ZeroVector;
	}
}


EFacingDirection USafeDirectionUtility::GetFacingDirectionFromAxisValue(float AxisValue)
{
	if (FMath::Abs(AxisValue) > DELTA)
	{
		if (AxisValue < 0.0f)
		{
			return EFacingDirection::LEFT;
		}
		else if (AxisValue > 0.0f)
		{
			return EFacingDirection::RIGHT;
		}
	}
	return EFacingDirection::NONE;
}
