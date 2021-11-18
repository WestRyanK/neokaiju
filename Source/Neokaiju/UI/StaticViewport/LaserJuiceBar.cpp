// Fill out your copyright notice in the Description page of Project Settings.


#include "LaserJuiceBar.h"

void ULaserJuiceBar::NativeConstruct() {
	KaijuCharacterEventSubsystem = this->GetWorld()->GetSubsystem<UKaijuCharacterEventSubsystem>();
	KaijuCharacterEventSubsystem->OnKaijuLaserChargeChanged.AddUniqueDynamic(this, &ULaserJuiceBar::UpdateRemainingCharge);
	Super::NativeConstruct();
}


void ULaserJuiceBar::NativeDestruct()
{
	KaijuCharacterEventSubsystem->OnKaijuLaserChargeChanged.RemoveDynamic(this, &ULaserJuiceBar::UpdateRemainingCharge);
	Super::NativeDestruct();
}

void ULaserJuiceBar::UpdateRemainingCharge(float OldChargeValue, float NewChargeValue, float MaxChargeValue)
{
	OldCharge = OldChargeValue;
	CurrentCharge = NewChargeValue;
	MaxCharge = MaxChargeValue;
	RefreshUI();
}