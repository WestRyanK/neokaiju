// Fill out your copyright notice in the Description page of Project Settings.

#include "CumulativePointPopup.h"

void UCumulativePointPopup::Setup(int currentMultiplier, float PropertyValue) {
	multiplier = currentMultiplier;
	score = PropertyValue;
	SetPositionAbovePlayer();
	AddToViewport(-1);
}

void UCumulativePointPopup::SetPositionAbovePlayer() {
	APlayerController* playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	FVector characterPosition = playerController->GetPawn()->GetActorLocation();
	FVector2D adjustedPosition;
	playerController->ProjectWorldLocationToScreen(characterPosition, adjustedPosition);
	adjustedPosition += playerOffset;
	SetPositionInViewport(adjustedPosition);
	positionInViewport = adjustedPosition;
}

void UCumulativePointPopup::ResetHUDActivePointPopup() {
	APlayerController* playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	Cast<AInGameHUD>(playerController->GetHUD())->ClearActivePointPopup();
}