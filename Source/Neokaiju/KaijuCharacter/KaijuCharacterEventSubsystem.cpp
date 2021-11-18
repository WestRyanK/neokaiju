// Fill out your copyright notice in the Description page of Project Settings.


#include "KaijuCharacterEventSubsystem.h"

void UKaijuCharacterEventSubsystem::KaijuDied()
{
	this->OnKaijuDied.Broadcast();
}

void UKaijuCharacterEventSubsystem::BabyKaijuSaved()
{
	this->OnBabyKaijuSaved.Broadcast();
}

void UKaijuCharacterEventSubsystem::KaijuHealthChanged(float OldHealth, float NewHealth, float MaxHealth)
{
	this->OnKaijuHealthChanged.Broadcast(OldHealth, NewHealth, MaxHealth);
}

void UKaijuCharacterEventSubsystem::KaijuLaserChargeChanged(float OldCharge, float NewCharge, float MaxCharge)
{
	this->OnKaijuLaserChargeChanged.Broadcast(OldCharge, NewCharge, MaxCharge);
}

void UKaijuCharacterEventSubsystem::KaijuTrappedByClaw()
{
	this->OnKaijuTrappedByClaw.Broadcast();
}

void UKaijuCharacterEventSubsystem::KaijuReleasedFromClaw()
{
	this->OnKaijuReleasedFromClaw.Broadcast();
}

//void UKaijuCharacterEventSubsystem::KaijuHealthRegenerationChanged(float NewHealthRegeneration, float MaxHealthRegeneration)
//{

//	UE_LOG(LogTemp, Display, TEXT("newRegenreatn %f"), NewHealthRegeneration);
//	this->OnKaijuHealthRegenerationChanged.Broadcast(NewHealthRegeneration, MaxHealthRegeneration);
//}
