// Fill out your copyright notice in the Description page of Project Settings.


#include "CameraViewTrigger.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Camera/CameraComponent.h"

ACameraViewTrigger::ACameraViewTrigger()
{
	this->CameraView = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraView"));
	this->CameraView->SetupAttachment(this->RootComponent);
	this->CameraView->SetAbsolute(true, true, true);
}

void ACameraViewTrigger::OnBeginTrigger(AActor* TriggeringActor)
{
	this->GetWorld()->GetFirstPlayerController()->SetViewTargetWithBlend(this, this->EaseInTime, this->EaseInBlendFunction, this->EaseInBlendExponent, true);
	this->GetWorld()->GetTimerManager().SetTimer(this->TransitioningTimerHandle, this, &ACameraViewTrigger::OnTransitionCompleted, this->EaseInTime, false);
}

void ACameraViewTrigger::OnEndTrigger(AActor* TriggeringActor)
{
	this->GetWorld()->GetFirstPlayerController()->SetViewTargetWithBlend(TriggeringActor, this->EaseOutTime, this->EaseOutBlendFunction, this->EaseOutBlendExponent, true);
	this->GetWorld()->GetTimerManager().SetTimer(this->TransitioningTimerHandle, this, &ACameraViewTrigger::OnTransitionCompleted, this->EaseOutTime, false);
}

void ACameraViewTrigger::OnTransitionCompleted()
{
	this->GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
	this->FinishTransition();
}
