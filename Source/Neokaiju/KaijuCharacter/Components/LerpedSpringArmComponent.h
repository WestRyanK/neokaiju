// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SpringArmComponent.h"
#include "LerpedSpringArmComponent.generated.h"

UENUM(BlueprintType)
enum class ELerpedSpringArmMotion : uint8
{
	None			UMETA(DisplayName = "None"),
	Extend			UMETA(DisplayName = "Extend"),
	Retract			UMETA(DisplayName = "Retract")
};
/**
 *
 */
UCLASS()
class NEOKAIJU_API ULerpedSpringArmComponent : public USpringArmComponent
{
	GENERATED_BODY()

public:
	// The speed at which to retract the SpringArm when it is blocked.
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float RetractSpeed = 100.0f;

	// The speed at which to re-extend the SpringArm when it is unblocked.
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float ExtendSpeed = 100.0f;


protected:

	virtual ELerpedSpringArmMotion GetLerpedSpringArmMotion(FVector CurrentSocketLocation, FVector Destination, FVector ComponentLocation);

	virtual float GetLerpSpeed(ELerpedSpringArmMotion Motion);

	virtual FVector BlendLocations(const FVector& DesiredArmLocation, const FVector& TraceHitLocation, bool bHitSomething, float DeltaTime);
};
