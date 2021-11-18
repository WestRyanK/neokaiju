// Fill out your copyright notice in the Description page of Project Settings.


#include "KaijuCharacterController.h"


void AKaijuCharacterController::SetupInputComponent()
{
	Super::SetupInputComponent();

	FInputActionBinding SkipIntroBinding = FInputActionBinding("RequestSkipIntro", EInputEvent::IE_Pressed);
	SkipIntroBinding.bConsumeInput = false;
	SkipIntroBinding.ActionDelegate.BindDelegate(this, &AKaijuCharacterController::RequestSkipIntro);
	InputComponent->AddActionBinding(SkipIntroBinding);

	FInputActionBinding PauseUnpauseBinding = FInputActionBinding("RequestPauseUnpause", EInputEvent::IE_Pressed);
	PauseUnpauseBinding.bExecuteWhenPaused = true;
	PauseUnpauseBinding.ActionDelegate.BindDelegate(this, &AKaijuCharacterController::RequestPauseUnpause);
	InputComponent->AddActionBinding(PauseUnpauseBinding);
}

void AKaijuCharacterController::RequestSkipIntro()
{
	this->OnSkipIntroRequested.Broadcast();
}

void AKaijuCharacterController::RequestPauseUnpause()
{
	this->OnPauseUnpauseRequested.Broadcast();
}
