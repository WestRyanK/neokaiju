// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthCounter.h"

void UHealthCounter::NativeConstruct() {
	KaijuCharacterEventSubsystem = this->GetWorld()->GetSubsystem<UKaijuCharacterEventSubsystem>();
	KaijuCharacterEventSubsystem->OnKaijuHealthChanged.AddUniqueDynamic(this, &UHealthCounter::UpdateHealth);
	//KaijuCharacterEventSubsystem->OnKaijuHealthRegenerationChanged.AddUniqueDynamic(this, &UHealthCounter::UpdateRegeneration);
	Super::NativeConstruct();
}

void UHealthCounter::NativeDestruct()
{
	KaijuCharacterEventSubsystem->OnKaijuHealthChanged.RemoveDynamic(this, &UHealthCounter::UpdateHealth);
	//KaijuCharacterEventSubsystem->OnKaijuHealthRegenerationChanged.RemoveDynamic(this, &UHealthCounter::UpdateRegeneration);
	Super::NativeDestruct();
}

void UHealthCounter::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UHealthCounter::UpdateHealth(float OldHealth, float NewHealth, float MaxHealth)
{
	currentHealth = NewHealth;
	maxHealth = MaxHealth;
	RefreshUI();
}
//void UHealthCounter::UpdateRegeneration(float NewHealthRegeneration, float MaxHealthRegeneration) {
//	currentHealthRegeneration = NewHealthRegeneration;
//	maxHealthRegeneration = MaxHealthRegeneration;
//	RefreshUI();
//}

void UHealthCounter::RefreshUI_Implementation() {
	if (healthIndicators.Num() == 0) {
		UE_LOG(LogTemp, Warning, TEXT("Health counter has not been provided any individual health counters"));
		return;
	}

	//Implementation is an artifact of the last HealthCounter system, based on float values
	float invisible = 0.0f;
	float available = 1.0f;

	//Set container visibilities
	size_t i = 0;
	for (; i < currentHealth; i++)
	{
		// this if statement prevents a crash that occurs when healing if the kaiju's max level is greater than the number of hearts displayed
		if (i < healthIndicators.Num())
		{
			healthIndicators[i]->SetPercentage(available);
		}
		
	}
	/*if (currentHealth != maxHealth) {
		healthIndicators[i]->SetPercentage(currentHealthRegeneration / maxHealthRegeneration);
		i++;
	}*/
	for (; i < healthIndicators.Num(); i++)
	{
		healthIndicators[i]->SetPercentage(invisible);
	}

}