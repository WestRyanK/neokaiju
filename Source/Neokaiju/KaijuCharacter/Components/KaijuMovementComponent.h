
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Neokaiju/Core/SafeDirectionUtility.h"
#include "GameplayTags.h"
#include "KaijuMovementComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFacingDirectionFlipped, EFacingDirection, FacingDirection);

class AKaijuCharacter;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class NEOKAIJU_API UKaijuMovementComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UKaijuMovementComponent();

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		bool bIsConstrainedToYAxis = true;

	// if true, the character will continue walking forever (used in endgame)
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool bLockWalkCycle = false;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		float ConstrainedYValue = 0.0f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
		FGameplayTagContainer MovementBlockingTags;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
		FGameplayTagContainer LeftRightFlipBlockingTags;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void ConstrainToAxis();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, BlueprintGetter = GetFacingDirection, BlueprintSetter = SetFacingDirection)
		EFacingDirection FacingDirection = EFacingDirection::RIGHT;

	AKaijuCharacter* Character;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
		void MoveKaijuCharacter(float AxisValue);

	UPROPERTY(BlueprintAssignable)
		FOnFacingDirectionFlipped OnFacingDirectionFlipped;

	void FlipFacingDirection();

	UFUNCTION(BlueprintGetter)
		EFacingDirection GetFacingDirection() const;

	UFUNCTION(BlueprintSetter)
		void SetFacingDirection(EFacingDirection NewFacingDirection);

	UFUNCTION(BlueprintCallable, BlueprintPure)
		FVector GetCharacterForward() const;

	UFUNCTION(BlueprintCallable)
		void SetCharacterForward(
			FVector ForwardVector,
			FVector TurnDirectionIfVertical180 = FVector::ZeroVector,
			FVector TurnDirectionIfNonVertical180 = FVector::ZeroVector,
			float RotateSpeed = 0.0f,
			float DeltaTime = 0.0f,
			bool bMaintainKaijuRightVector = false);

		UFUNCTION(BlueprintCallable)
		void RotateCharacterRelative(FRotator RelativeRotation, bool bCanFlipCharacter = true);


};
