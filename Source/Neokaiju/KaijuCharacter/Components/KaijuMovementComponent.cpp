// Fill out your copyright notice in the Description page of Project Settings.


#include "KaijuMovementComponent.h"
#include "Neokaiju/KaijuCharacter/KaijuCharacter.h"
#include "Neokaiju/KaijuCharacter/Components/EffectTriggerLineTraceComponent.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values for this component's properties
UKaijuMovementComponent::UKaijuMovementComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UKaijuMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	this->Character = Cast<AKaijuCharacter>(this->GetOwner());
	// ...

}


// Called every frame
void UKaijuMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	this->ConstrainToAxis();
	// ...
}


void UKaijuMovementComponent::ConstrainToAxis()
{
	if (this->bIsConstrainedToYAxis && IsValid(this->GetOwner()))
	{
		FVector Location = this->GetOwner()->GetActorLocation();
		Location.Y = this->ConstrainedYValue;
		this->GetOwner()->SetActorLocation(Location);
	}
}

void UKaijuMovementComponent::FlipFacingDirection()
{
	if (this->FacingDirection == EFacingDirection::RIGHT)
	{
		this->SetFacingDirection(EFacingDirection::LEFT);
	}
	else
	{
		this->SetFacingDirection(EFacingDirection::RIGHT);
	}
}
EFacingDirection UKaijuMovementComponent::GetFacingDirection() const
{
	return this->FacingDirection;
}

void UKaijuMovementComponent::SetFacingDirection(EFacingDirection NewFacingDirection)
{
	// Cancel if already facing the determined direction
	if (this->FacingDirection != NewFacingDirection && NewFacingDirection != EFacingDirection::NONE)
	{
		this->FacingDirection = NewFacingDirection;

		float Yaw;
		FVector MirrorScale;
		if (NewFacingDirection == EFacingDirection::RIGHT)
		{
			Yaw = 0.0f;
			MirrorScale = FVector(1.0f, 1.0f, 1.0f);
		}
		else
		{
			Yaw = 180.0f;
			MirrorScale = FVector(1.0f, -1.0f, 1.0f);
		}

		// Flip
		FRotator PreviousRotation = this->Character->CharacterRotationPoint->GetComponentRotation();

		this->Character->CharacterRotationPoint->SetWorldRotation(FRotator(PreviousRotation.Pitch, Yaw, 0.0f));
		this->Character->FacingWallTrigger->SetWorldRotation(FRotator(0, Yaw, 0));
		FVector OriginalScale = this->Character->GetMesh()->GetRelativeScale3D().GetAbs();
		this->Character->GetMesh()->SetRelativeScale3D(MirrorScale * OriginalScale);

		// Inform Blueprints
		this->OnFacingDirectionFlipped.Broadcast(NewFacingDirection);
		//this->OnFacingDirectionFlipped(NewFacingDirection);
	}
}

FVector UKaijuMovementComponent::GetCharacterForward() const
{
	return this->Character->CharacterRotationPoint->GetForwardVector();
}

void UKaijuMovementComponent::SetCharacterForward(
	FVector ForwardVector,
	FVector TurnDirectionIfVertical180,
	FVector TurnDirectionIfNonVertical180,
	float RotateSpeed,
	float DeltaTime,
	bool bMaintainKaijuRightVector)
{
	FRotator Rotator = USafeDirectionUtility::GetSafePitchRotation(
		this->Character->CharacterRotationPoint->GetRightVector(),
		this->Character->CharacterRotationPoint->GetForwardVector(),
		ForwardVector,
		TurnDirectionIfVertical180,
		TurnDirectionIfNonVertical180,
		RotateSpeed,
		DeltaTime,
		bMaintainKaijuRightVector);

	this->Character->CharacterRotationPoint->SetWorldRotation(Rotator);
	if (!bMaintainKaijuRightVector)
	{
		this->SetFacingDirection(USafeDirectionUtility::GetFacingDirectionFromAxisValue(this->Character->CharacterRotationPoint->GetForwardVector().X));
	}
}

void UKaijuMovementComponent::RotateCharacterRelative(FRotator RelativeRotation, bool bCanFlipCharacter)
{
	this->Character->CharacterRotationPoint->AddLocalRotation(RelativeRotation, true);
	if (bCanFlipCharacter)
	{
		this->SetFacingDirection(USafeDirectionUtility::GetFacingDirectionFromAxisValue(this->Character->CharacterRotationPoint->GetForwardVector().X));
	}
}

void UKaijuMovementComponent::MoveKaijuCharacter(float AxisValue)
{
	bool bCanLeftRightFlip = !this->Character->GetAbilitySystemComponent()->HasAnyMatchingGameplayTags(this->LeftRightFlipBlockingTags);
	bool bCanMove = !this->Character->GetAbilitySystemComponent()->HasAnyMatchingGameplayTags(this->MovementBlockingTags);

	if (bLockWalkCycle)
	{
		AxisValue = 1.0f;
	}

	if (bCanMove || bLockWalkCycle)
	{
		// I don't like that we are casting and updating this value every frame, but it probably works well enough for now...
		UCharacterMovementComponent* MoveComp = Cast<UCharacterMovementComponent>(Character->GetMovementComponent());
		if (MoveComp)
		{
			MoveComp->MaxWalkSpeed = Character->GetWalkSpeed();
		}
		this->Character->AddMovementInput(FVector(1, 0, 0), AxisValue, false);

		// Face character in direction of movement.
		FRotator PreviousRotation = this->Character->CharacterRotationPoint->GetComponentRotation();
		if (bCanLeftRightFlip)
		{
			this->SetFacingDirection(USafeDirectionUtility::GetFacingDirectionFromAxisValue(AxisValue));
		}
	}
}

