// Fill out your copyright notice in the Description page of Project Settings.


#include "AimComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/Actor.h"
#include "DrawDebugHelpers.h"

// Sets default values for this component's properties
UAimComponent::UAimComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

void UAimComponent::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(this->GetOwner()) && IsValid(this->GetOwner()->InputComponent))
	{
		this->GetOwner()->InputComponent->BindAxis(TEXT("Right"));
		this->GetOwner()->InputComponent->BindAxis(TEXT("Up"));
		this->GetOwner()->InputComponent->BindAxis(TEXT("CameraRight"));
		this->GetOwner()->InputComponent->BindAxis(TEXT("CameraUp"));
	}
}

// Called every frame
void UAimComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	this->UpdateAimDirection();
	this->UpdateAimArrow();
}

void UAimComponent::UpdateAimDirection()
{
	if (IsValid(this->GetOwner()) && IsValid(this->GetOwner()->InputComponent))
	{
		float RightAxis = this->GetOwner()->InputComponent->GetAxisValue(TEXT("Right"));
		float UpAxis = this->GetOwner()->InputComponent->GetAxisValue(TEXT("Up"));

		this->PrimaryDirection = FVector(RightAxis, 0, UpAxis);

		if (this->PrimaryDirection.SizeSquared() < SMALL_NUMBER) // SizeSquared is more efficient than Size. Thanks math
		{
			this->PrimaryDirection = FVector::ForwardVector * FMath::Sign(this->DefaultAimForwardDirectionComponent->GetForwardVector().X);
			//this->PrimaryDirection = FVector::ForwardVector * FMath::Sign(this->GetOwner()->GetActorForwardVector().X);
			this->bIsAimingPrimary = false;
		}
		else
		{
			this->bIsAimingPrimary = true;
		}

		this->PrimaryDirection.Normalize();

		RightAxis = this->GetOwner()->InputComponent->GetAxisValue(TEXT("CameraRight"));
		UpAxis = this->GetOwner()->InputComponent->GetAxisValue(TEXT("CameraUp"));

		this->SecondaryDirection = FVector(RightAxis, 0, -UpAxis);

		if (this->SecondaryDirection.SizeSquared() < SMALL_NUMBER) // SizeSquared is more efficient than Size. Thanks math
		{
			this->SecondaryDirection = FVector::ForwardVector * FMath::Sign(this->DefaultAimForwardDirectionComponent->GetForwardVector().X);
			//this->AimDirection = FVector::ForwardVector * FMath::Sign(this->GetOwner()->GetActorForwardVector().X);
			this->bIsAimingSecondary = false;
		}
		else
		{
			this->bIsAimingSecondary = true;
		}

		this->SecondaryDirection.Normalize();
	}
}

void UAimComponent::UpdateAimArrow()
{
#if !UE_BUILD_SHIPPING // Don't draw arrow when shipping
	if (this->bShouldDrawAimDirectionInDebug)
	{
		FVector LineStart = this->GetOwner()->GetActorLocation();
		FVector LineEnd = LineStart + this->DebugLineLength * this->PrimaryDirection;
		DrawDebugLine(this->GetWorld(), LineStart, LineEnd, FColor::Green, false, 0.0f, 0, this->DebugLineThickness);

		LineEnd = LineStart + this->DebugLineLength * this->SecondaryDirection;
		DrawDebugLine(this->GetWorld(), LineStart, LineEnd, FColor::Blue, false, 0.0f, 0, this->DebugLineThickness);
	}
#endif
}
