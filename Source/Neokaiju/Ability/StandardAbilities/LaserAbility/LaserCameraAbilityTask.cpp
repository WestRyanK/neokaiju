// Fill out your copyright notice in the Description page of Project Settings.


#include "LaserCameraAbilityTask.h"
#include "Neokaiju/KaijuCharacter/KaijuCharacter.h"
#include "Camera/CameraComponent.h"
#include "Camera/CameraShake.h"
#include "Kismet/GameplayStatics.h"



ULaserCameraAbilityTask::ULaserCameraAbilityTask()
{
	this->bTickingTask = true;
}

ULaserCameraAbilityTask* ULaserCameraAbilityTask::LaserCamera(
	UGameplayAbility* OwningAbility,
	TSubclassOf<UCameraShake> CameraShake,
	float LaserCameraOffset,
	bool bShouldCenterCameraOnActor)
{
	ULaserCameraAbilityTask* LaserCameraAbilityTask = NewAbilityTask<ULaserCameraAbilityTask>(OwningAbility);
	LaserCameraAbilityTask->LaserCameraOffset = LaserCameraOffset;
	LaserCameraAbilityTask->CameraShake = CameraShake;
	LaserCameraAbilityTask->bShouldCenterCameraOnActor = bShouldCenterCameraOnActor;
	return LaserCameraAbilityTask;
}

void ULaserCameraAbilityTask::Activate()
{
	Super::Activate();

	AKaijuCharacter* Character = Cast<AKaijuCharacter>(this->GetAvatarActor());
	if (IsValid(Character))
	{
		this->CameraFocus = Character->CameraFocusPoint;
		if (IsValid(this->CameraFocus))
		{
			this->OriginalCameraLocation = this->CameraFocus->GetRelativeLocation();
			ABTASK_LOG(TEXT("Getting Camera Location %s"), *this->OriginalCameraLocation.ToString());
			if (this->bShouldCenterCameraOnActor)
			{
				this->CameraPivotCenter = Character->GetActorLocation();
			}
			else
			{
				this->CameraPivotCenter = this->CameraFocus->GetComponentLocation();
			}
		}
	}
}

void ULaserCameraAbilityTask::TickTask(float DeltaTime)
{
	this->UpdateCameraPosition(DeltaTime);

	if (IsValid(this->CameraShake))
	{
		UGameplayStatics::PlayWorldCameraShake(this, this->CameraShake, this->CameraFocus->GetComponentLocation(), 0, 1000.0f);
	}
}

void ULaserCameraAbilityTask::UpdateCameraPosition(float DeltaTime)
{
	if (IsValid(this->CameraFocus))
	{
		FVector CameraDestination = this->LaserAimDirection * this->LaserCameraOffset;
		CameraDestination += this->CameraPivotCenter;

		this->CameraFocus->SetWorldLocation(CameraDestination);
	}
}

void ULaserCameraAbilityTask::FireLaserComplete()
{
	EndTask();
}

void ULaserCameraAbilityTask::SetLaserAimDirection(FVector LaserAimDirectionValue)
{
	this->LaserAimDirection = LaserAimDirectionValue;
}

void ULaserCameraAbilityTask::OnDestroy(bool bInOwnerFinished)
{
	this->CameraFocus->SetRelativeLocation(this->OriginalCameraLocation);
	ABTASK_LOG(TEXT("Resetting camera to %s"), *this->OriginalCameraLocation.ToString());
	Super::OnDestroy(bInOwnerFinished);
}
