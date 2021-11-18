// Fill out your copyright notice in the Description page of Project Settings.


#include "ShakerComponent.h"
#include "Camera/CameraShake.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"

UShakerComponent::UShakerComponent()
{
	this->PrimaryComponentTick.bCanEverTick = true;
	this->SetTickEnabled(true);
}

void UShakerComponent::DemoShakeInEditor()
{
	this->bIsEditorDemo = true;
	this->PlayShakeOnActor();
}

void UShakerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	ShakeTick(DeltaTime);
}

void UShakerComponent::PlayShakeOnActor()
{
	if (!this->bIsCurrentlyShaking)
	{
		this->OriginalPosition = FVector::ZeroVector;
		this->OriginalRotation = FRotator::ZeroRotator;
		if (IsValid(this->GetOwner()))
		{
			this->OriginalPosition = this->GetOwner()->GetActorLocation();
			this->OriginalRotation = this->GetOwner()->GetActorRotation();
		}
	}

	this->bIsCurrentlyShaking = true;
	this->ShakeTimeElapsed = 0.0f;

	this->Shake = this->ShakeClass.GetDefaultObject();
	if (IsValid(this->Shake))
	{
		this->CurrentPositionTimeOffset.X = FMath::Rand() * PI * 2;
		this->CurrentPositionTimeOffset.Y = FMath::Rand() * PI * 2;
		this->CurrentPositionTimeOffset.Z = FMath::Rand() * PI * 2;

		this->CurrentRotationTimeOffset.X = FMath::Rand() * PI * 2;
		this->CurrentRotationTimeOffset.Y = FMath::Rand() * PI * 2;
		this->CurrentRotationTimeOffset.Z = FMath::Rand() * PI * 2;
	}

	//this->SetTickEnabled(true);
}

void UShakerComponent::ShakeTick(float DeltaTime)
{

	if (this->bIsCurrentlyShaking && IsValid(this->Shake))
	{
		this->ShakeTimeElapsed += DeltaTime;

		if (this->ShakeTimeElapsed > this->Shake->Duration)
		{
			this->EndShake();
			return;
		}

		float DeltaOffset = DeltaTime * PI * 2;
		FVector PositionOffset;
		PositionOffset.X = FFOscillator::UpdateOffset(this->Shake->PositionOscillator.X, this->CurrentPositionTimeOffset.X, DeltaOffset);
		PositionOffset.Y = FFOscillator::UpdateOffset(this->Shake->PositionOscillator.Y, this->CurrentPositionTimeOffset.Y, DeltaOffset);
		PositionOffset.Z = FFOscillator::UpdateOffset(this->Shake->PositionOscillator.Z, this->CurrentPositionTimeOffset.Z, DeltaOffset);

		FRotator RotationOffset;
		RotationOffset.Roll = FFOscillator::UpdateOffset(this->Shake->RotationOscillator.Roll, this->CurrentRotationTimeOffset.X, DeltaOffset);
		RotationOffset.Pitch = FFOscillator::UpdateOffset(this->Shake->RotationOscillator.Pitch, this->CurrentRotationTimeOffset.Y, DeltaOffset);
		RotationOffset.Yaw = FFOscillator::UpdateOffset(this->Shake->RotationOscillator.Yaw, this->CurrentRotationTimeOffset.Z, DeltaOffset);

		if (IsValid(this->GetOwner()))
		{
			if (this->Shake->PositionOscillator.X.Amplitude > 0 || this->Shake->PositionOscillator.Y.Amplitude > 0 || this->Shake->PositionOscillator.Z.Amplitude > 0)
			{
				this->GetOwner()->SetActorLocation(PositionOffset + this->OriginalPosition);
			}
			this->GetOwner()->SetActorRotation(RotationOffset + this->OriginalRotation);
		}
	}
}

void UShakerComponent::EndShake()
{
	if (this->bIsCurrentlyShaking)
	{
		if (this->Shake->bReturnsToOriginalTransform || this->bIsEditorDemo)
		{
			if (IsValid(this->GetOwner()))
			{
				this->GetOwner()->SetActorLocation(this->OriginalPosition);
				this->GetOwner()->SetActorRotation(this->OriginalRotation);
			}
		}

		this->Shake = nullptr;
		this->bIsCurrentlyShaking = false;
		this->bIsEditorDemo = false;

		//this->SetTickEnabled(false);
	}
}

void UShakerComponent::SetTickEnabled(bool TickEnabled)
{
	this->bTickInEditor = TickEnabled;
	this->SetComponentTickEnabled(TickEnabled);
	this->PrimaryComponentTick.SetTickFunctionEnable(TickEnabled);
}
