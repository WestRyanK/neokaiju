// Fill out your copyright notice in the Description page of Project Settings.


#include "CameraPanComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/Actor.h"

const FName UCameraPanComponent::SocketName(TEXT("CameraPanSocket"));

// Sets default values for this component's properties
UCameraPanComponent::UCameraPanComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCameraPanComponent::BeginPlay()
{
	Super::BeginPlay();

	/*if (IsValid(this->GetOwner()) && IsValid(this->GetOwner()->InputComponent))
	{
		this->GetOwner()->InputComponent->BindAxis(TEXT("CameraRight"));
		this->GetOwner()->InputComponent->BindAxis(TEXT("CameraUp"));
	}*/

}


// Called every frame
void UCameraPanComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	/*float RightCameraAxis = this->GetOwner()->InputComponent->GetAxisValue(TEXT("CameraRight"));
	float UpCameraAxis = this->GetOwner()->InputComponent->GetAxisValue(TEXT("CameraUp"));

	this->CameraPanOffset += FVector(-RightCameraAxis * this->PanSpeed, 0.0f, -UpCameraAxis * this->PanSpeed);
	float X = FMath::Clamp(this->CameraPanOffset.X, -this->MaxCameraOffset.X, this->MaxCameraOffset.X);
	float Y = FMath::Clamp(this->CameraPanOffset.Y, -this->MaxCameraOffset.Y, this->MaxCameraOffset.Y);
	float Z = FMath::Clamp(this->CameraPanOffset.Z, -this->MaxCameraOffset.Z, this->MaxCameraOffset.Z);
	this->CameraPanOffset = FVector(X, Y, Z);

	if (FMath::IsNearlyZero(RightCameraAxis) && FMath::IsNearlyZero(UpCameraAxis))
	{
		FVector OriginReturnOffset = FVector(
			this->GetReturnComponent(X),
			this->GetReturnComponent(Y),
			this->GetReturnComponent(Z));

		this->CameraPanOffset += OriginReturnOffset;
	}*/
}

float UCameraPanComponent::GetReturnComponent(float AxisValue)
{
	float OriginReturnOffset = FMath::Min(this->OriginReturnSpeed, FMath::Abs(AxisValue)) * -FMath::Sign(AxisValue);
	return OriginReturnOffset;
}

FTransform UCameraPanComponent::GetSocketTransform(FName InSocketName, ERelativeTransformSpace TransformSpace) const
{
	FTransform RelativeTransform = FTransform(this->CameraPanOffset);

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

bool UCameraPanComponent::HasAnySockets() const
{
	return true;
}

void UCameraPanComponent::QuerySupportedSockets(TArray<FComponentSocketDescription>& OutSockets) const
{
	new (OutSockets) FComponentSocketDescription(SocketName, EComponentSocketType::Socket);
}


