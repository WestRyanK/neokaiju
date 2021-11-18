// Fill out your copyright notice in the Description page of Project Settings.


#include "LerpedSpringArmComponent.h"


FVector ULerpedSpringArmComponent::BlendLocations(const FVector& DesiredArmLocation, const FVector& TraceHitLocation, bool bHitSomething, float DeltaTime)
{
	FVector CurrentSocketLocation = this->GetSocketLocation(USpringArmComponent::SocketName);
	FVector Destination;
	if (bHitSomething)
	{
		Destination = TraceHitLocation;
	}
	else
	{
		Destination = DesiredArmLocation;
	}


	FVector ComponentLocation = this->GetComponentLocation();

	FVector DestinationCurrentLocationDifference = Destination - CurrentSocketLocation;
	float DistanceFromDestination = DestinationCurrentLocationDifference.Size();
	FVector DirectionToDestination = DestinationCurrentLocationDifference.GetSafeNormal();


	ELerpedSpringArmMotion Motion = this->GetLerpedSpringArmMotion(CurrentSocketLocation, Destination, ComponentLocation);
	float LerpSpeed = this->GetLerpSpeed(Motion);

	FVector Offset = DirectionToDestination * DeltaTime * LerpSpeed;
	float OffsetDistance = Offset.Size();

	if (OffsetDistance >= DistanceFromDestination)
	{
		return Destination;
	}
	else
	{
		return CurrentSocketLocation + Offset;
	}
}

float ULerpedSpringArmComponent::GetLerpSpeed(ELerpedSpringArmMotion Motion)
{
	switch (Motion)
	{
	case ELerpedSpringArmMotion::None:
		return 0.0f;
	case ELerpedSpringArmMotion::Extend:
		return this->ExtendSpeed;
	case ELerpedSpringArmMotion::Retract:
		return this->RetractSpeed;
	default:
		return 0.0f;
	}
}

ELerpedSpringArmMotion ULerpedSpringArmComponent::GetLerpedSpringArmMotion(FVector CurrentSocketLocation, FVector Destination, FVector ComponentLocation)
{
	float CurrentSpringArmLength = (CurrentSocketLocation - ComponentLocation).Size();
	float DestinationSpringArmLength = (Destination - ComponentLocation).Size();
	if (DestinationSpringArmLength > CurrentSpringArmLength)
	{
		return ELerpedSpringArmMotion::Extend;
	}
	else if (DestinationSpringArmLength < CurrentSpringArmLength)
	{
		return ELerpedSpringArmMotion::Retract;
	}
	else
	{
		return ELerpedSpringArmMotion::None;
	}
}
