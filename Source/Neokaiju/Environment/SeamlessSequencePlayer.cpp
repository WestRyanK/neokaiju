// Fill out your copyright notice in the Description page of Project Settings.


#include "SeamlessSequencePlayer.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include "Camera/CameraActor.h"
#include "LevelSequenceActor.h"
#include "LevelSequencePlayer.h"
#include "MovieSceneSequencePlayer.h"


USeamlessSequencePlayer::USeamlessSequencePlayer()
{

}


void USeamlessSequencePlayer::StopSequence()
{
	if (IsValid(this->Sequence))
	{
		UWorld* World = this->GetWorld();
		if (IsValid(World))
		{
			World->GetTimerManager().ClearTimer(this->EaseInTimerHandle);
			World->GetTimerManager().ClearTimer(this->EaseOutTimerHandle);
			World->GetTimerManager().ClearTimer(this->PlaySequenceTimerHandle);
			this->Sequence->GetSequencePlayer()->Stop();
			World->GetFirstPlayerController()->SetViewTarget(this->OriginalViewTarget);
			this->OnEaseOutCompleted();
		}
	}
}

void USeamlessSequencePlayer::PlaySequence()
{
	UWorld* World = this->GetWorld();
	if (IsValid(World))
	{
		if (this->bIsInputDisabledDuringSequence)
		{
			ACharacter* Character = UGameplayStatics::GetPlayerCharacter(this, 0);
			Character->DisableInput(nullptr);
		}

		if (IsValid(Sequence))
		{
			if (IsValid(this->SequenceCamera))
			{
				World->GetFirstPlayerController()->SetViewTargetWithBlend(this->SequenceCamera, this->EaseInTime, this->EaseInBlendFunction, this->EaseInBlendExponent);
			}

			if (this->EaseInTime > 0.0f && IsValid(this->SequenceCamera))
			{
				World->GetTimerManager().SetTimer(this->EaseInTimerHandle, this, &USeamlessSequencePlayer::OnEaseInCompleted, this->EaseInTime, false);
			}
			else
			{
				this->OnEaseInCompleted();
			}
		}
		else
		{
			this->OnEaseOutCompleted();
		}
	}
}

void USeamlessSequencePlayer::InitFromTags(FName SequenceTag, FName CameraTag)
{
	this->Sequence = this->GetActorWithTag<ALevelSequenceActor>(ALevelSequenceActor::StaticClass(), SequenceTag);
	this->SequenceCamera = this->GetActorWithTag<ACameraActor>(ACameraActor::StaticClass(), CameraTag);
}

void USeamlessSequencePlayer::InitFromRefs(ALevelSequenceActor* SequenceValue, ACameraActor* SequenceCameraValue)
{
	this->Sequence = SequenceValue;
	this->SequenceCamera = SequenceCameraValue;
}

void USeamlessSequencePlayer::InitCommon(
	float EaseInTimeValue,
	float EaseOutTimeValue,
	bool bIsInputDisabledDuringSequenceValue,
	EViewTargetBlendFunction EaseInBlendFunctionValue,
	EViewTargetBlendFunction EaseOutBlendFunctionValue,
	float EaseInBlendExponentValue,
	float EaseOutBlendExponentValue)
{
	UWorld* World = this->GetWorld();
	if (IsValid(World))
	{
		this->EaseInTime = EaseInTimeValue;
		this->EaseOutTime = EaseOutTimeValue;
		this->EaseInBlendFunction = EaseInBlendFunctionValue;
		this->EaseOutBlendFunction = EaseOutBlendFunctionValue;
		this->EaseInBlendExponent = EaseInBlendExponentValue;
		this->EaseOutBlendExponent = EaseOutBlendExponentValue;
		this->bIsInputDisabledDuringSequence = bIsInputDisabledDuringSequenceValue;
		this->OriginalViewTarget = World->GetFirstPlayerController()->GetViewTarget();
	}
}

USeamlessSequencePlayer* USeamlessSequencePlayer::CreateSeamlessSequencePlayerFromTags(
	UObject* WorldContextObject,
	FName SequenceTag,
	FName CameraTag,
	float EaseInTimeValue,
	float EaseOutTimeValue,
	bool bIsInputDisabledDuringSequenceValue,
	EViewTargetBlendFunction EaseInBlendFunctionValue,
	EViewTargetBlendFunction EaseOutBlendFunctionValue,
	float EaseInBlendExponentValue,
	float EaseOutBlendExponentValue)
{
	if (IsValid(WorldContextObject))
	{
		USeamlessSequencePlayer* Player = NewObject<USeamlessSequencePlayer>(WorldContextObject);
		Player->InitCommon(
			EaseInTimeValue,
			EaseOutTimeValue,
			bIsInputDisabledDuringSequenceValue,
			EaseInBlendFunctionValue,
			EaseOutBlendFunctionValue,
			EaseInBlendExponentValue,
			EaseOutBlendExponentValue);
		Player->InitFromTags(SequenceTag, CameraTag);
		return Player;
	}
	else
	{
		return nullptr;
	}
}

USeamlessSequencePlayer* USeamlessSequencePlayer::CreateSeamlessSequencePlayerFromRefs(
	UObject* WorldContextObject,
	ALevelSequenceActor* Sequence,
	ACameraActor* SequenceCamera,
	float EaseInTimeValue,
	float EaseOutTimeValue,
	bool bIsInputDisabledDuringSequenceValue,
	EViewTargetBlendFunction EaseInBlendFunctionValue,
	EViewTargetBlendFunction EaseOutBlendFunctionValue,
	float EaseInBlendExponentValue,
	float EaseOutBlendExponentValue)
{
	if (IsValid(WorldContextObject))
	{
		USeamlessSequencePlayer* Player = NewObject<USeamlessSequencePlayer>(WorldContextObject);
		Player->InitCommon(
			EaseInTimeValue,
			EaseOutTimeValue,
			bIsInputDisabledDuringSequenceValue,
			EaseInBlendFunctionValue,
			EaseOutBlendFunctionValue,
			EaseInBlendExponentValue,
			EaseOutBlendExponentValue);
		Player->InitFromRefs(Sequence, SequenceCamera);
		return Player;
	}
	else
	{
		return nullptr;
	}
}

void USeamlessSequencePlayer::OnEaseInCompleted()
{
	// We need to begin the easing out BEFORE the sequence has finished or else the camera will snap back to its original position before the sequence ran.
	this->GetWorld()->GetTimerManager().SetTimer(
		this->PlaySequenceTimerHandle,
		this,
		&USeamlessSequencePlayer::OnSequenceFinished,
		this->Sequence->GetSequencePlayer()->GetDuration().AsSeconds() - this->EaseOutTime,
		false);
	this->Sequence->GetSequencePlayer()->Play();
}

void USeamlessSequencePlayer::OnEaseOutCompleted()
{
	if (this->bIsInputDisabledDuringSequence)
	{
		ACharacter* Character = UGameplayStatics::GetPlayerCharacter(this, 0);
		Character->EnableInput(nullptr);
	}

	this->OnSeamlessSequencePlayerFinished.Broadcast();
}

void USeamlessSequencePlayer::OnSequenceFinished()
{
	UWorld* World = this->GetWorld();
	if (IsValid(this->SequenceCamera))
	{
		World->GetFirstPlayerController()->SetViewTarget(this->SequenceCamera);
		World->GetFirstPlayerController()->SetViewTargetWithBlend(this->OriginalViewTarget, this->EaseOutTime, this->EaseOutBlendFunction, this->EaseOutBlendExponent);
	}

	if (this->EaseOutTime > 0.0f && IsValid(this->SequenceCamera))
	{
		World->GetTimerManager().SetTimer(this->EaseOutTimerHandle, this, &USeamlessSequencePlayer::OnEaseOutCompleted, this->EaseOutTime, false);
	}
	else
	{
		this->OnEaseOutCompleted();
	}
}
