// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AimComponent.generated.h"


/**
*  Monitors and reports the direction that the character is currently trying to aim with the joystick.
*/
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class NEOKAIJU_API UAimComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAimComponent();


	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// The direction that the user is currently aiming. 
	// Represents the direction the joystick is facing, or if
	// the joystick is in neutral position, the forward vector of the character.
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		FVector PrimaryDirection;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		FVector SecondaryDirection;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		bool bIsAimingPrimary = false;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		bool bIsAimingSecondary = false;

	// If not in shipping build, should a line be drawn from the center of the character
	// in the AimDirection?
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		bool bShouldDrawAimDirectionInDebug = true;

	// How thick to draw the DebugLine
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		float DebugLineThickness = 2.5f;

	// How long to draw the DebugLine
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		float DebugLineLength = 50.0f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		USceneComponent* DefaultAimForwardDirectionComponent;

protected:

	virtual void BeginPlay() override;

	void UpdateAimDirection();

	void UpdateAimArrow();
	
};
