// Fill out your copyright notice in the Description page of Project Settings.


#include "SocketSplineComponent.h"
#include "Engine/World.h"
#include "Neokaiju/Core/LerpUtility.h"


const FName USocketSplineComponent::SocketName(TEXT("SplineSocket"));

USocketSplineComponent::USocketSplineComponent()
{
}

void USocketSplineComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	this->UpdateSplineSocket();
}

float USocketSplineComponent::GetSplineSocketPositionKey()
{
	return this->SplineSocketPositionKey;
}

void USocketSplineComponent::SetSplineSocketPositionKey(float SplineSocketPositionKeyValue)
{
	this->SplineSocketPositionKey = SplineSocketPositionKeyValue;
	this->UpdateSplineSocket();
}

void USocketSplineComponent::SetSplineSocketPositionKeyDelayed(float SplineSocketPositionKeyValue)
{
	if (IsValid(this->LerpUtility))
	{
		this->LerpUtility->End(ELerpSetValue::CurrentValue);
	}

	this->LerpUtility = ULerpUtility::LerpFloatOverTime(
		this,
		this,
		this->SplineSocketPositionKey,
		SplineSocketPositionKeyValue,
		this->TransitionDuration,
		this->TransitionUpdateTickInterval,
		this->TransitionDelay);

	this->LerpUtility->FloatUpdated.AddUniqueDynamic(this, &USocketSplineComponent::SetSplineSocketPositionKey);
	this->LerpUtility->Begin();
}

void USocketSplineComponent::UpdateSplineSocket()
{
	this->RelativeSocketLocation = this->GetLocationAtSplineInputKey(this->SplineSocketPositionKey, ESplineCoordinateSpace::Local);
	this->UpdateChildTransforms();
}

FTransform USocketSplineComponent::GetSocketTransform(FName InSocketName, ERelativeTransformSpace TransformSpace) const
{
	FTransform RelativeTransform(RelativeSocketRotation, RelativeSocketLocation);

	switch (TransformSpace)
	{
	case RTS_World:
	{
		return RelativeTransform * GetComponentTransform();
		break;
	}
	case RTS_Actor:
	{
		if (const AActor* Actor = GetOwner())
		{
			FTransform SocketTransform = RelativeTransform * GetComponentTransform();
			return SocketTransform.GetRelativeTransform(Actor->GetTransform());
		}
		break;
	}
	case RTS_Component:
	{
		return RelativeTransform;
	}
	}
	return RelativeTransform;
}

bool USocketSplineComponent::HasAnySockets() const
{
	return true;
}

void USocketSplineComponent::QuerySupportedSockets(TArray<FComponentSocketDescription>& OutSockets) const
{
	new (OutSockets) FComponentSocketDescription(SocketName, EComponentSocketType::Socket);
}

void USocketSplineComponent::PostInitProperties()
{
	Super::PostInitProperties();
	this->UpdateSplineSocket();
}
