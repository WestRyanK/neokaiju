// Fill out your copyright notice in the Description page of Project Settings.


#include "MoveClawComponent.h"
#include "Curves/CurveFloat.h"
#include "Components/SkeletalMeshComponent.h"

// Sets default values for this component's properties
UMoveClawComponent::UMoveClawComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMoveClawComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	this->SetComponentTickEnabled(false);
}


// Called every frame
void UMoveClawComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	MoveClawExecution(DeltaTime);
}


void UMoveClawComponent::MoveClaw(UCurveFloat* inMovementCurve, USkeletalMeshComponent* inClaw, FVector inEnd, float inClawSpeed, bool inStopOnNonObjectiveHit)
{
	this->MovementCurve = inMovementCurve;
	this->Claw = inClaw;
	this->End = inEnd;
	this->ClawSpeed = inClawSpeed;
	this->StopOnNonObjectiveHit = inStopOnNonObjectiveHit;

	//UE_LOG(LogTemp, Display, TEXT("Setting up timeline"));

	this->StartTime = GetWorld()->TimeSeconds;
	this->SetComponentTickEnabled(true);
}


void UMoveClawComponent::MoveClawExecution(float DeltaTime)
{
	float Value = this->MovementCurve->GetFloatValue(GetWorld()->TimeSeconds - StartTime);
	float CurrentSpeed = Value * ClawSpeed;

	float Distance = CurrentSpeed * DeltaTime;
	FVector Direction = (this->End - this->Claw->GetComponentLocation());
	Direction.Normalize();

	FVector NewLocation = this->Claw->GetComponentLocation() + (Direction * Distance);

	//UE_LOG(LogTemp, Display, TEXT("Updating Timeline: %f "), Value);
	this->Claw->SetWorldLocation(NewLocation, true, nullptr, ETeleportType::TeleportPhysics);

	if ((NewLocation - this->End).Size() < 10.0f)
	{
		Stop();
	}
}

void UMoveClawComponent::Stop()
{
	this->SetComponentTickEnabled(false);
	this->ReachedEndEvent.Broadcast();
}
