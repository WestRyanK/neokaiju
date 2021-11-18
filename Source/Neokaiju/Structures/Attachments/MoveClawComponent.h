// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MoveClawComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FReachedEndEvent);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class NEOKAIJU_API UMoveClawComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMoveClawComponent();

	UPROPERTY(BlueprintAssignable, Category = "Event")
		FReachedEndEvent ReachedEndEvent;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
		void MoveClaw(class UCurveFloat* inMovementCurve, USkeletalMeshComponent* inClaw, FVector inEnd, float inClawSpeed, bool inStopOnNonObjectiveHit);

	UFUNCTION()
		void MoveClawExecution(float DeltaTime);

	UFUNCTION()
		void Stop();

private:
	float StartTime;

	UCurveFloat* MovementCurve;
	USkeletalMeshComponent* Claw;
	FVector End;
	float ClawSpeed;
	bool StopOnNonObjectiveHit;
	TArray<AActor*> Objectives;
};
