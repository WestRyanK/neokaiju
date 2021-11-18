// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthWarning.h"

void UHealthWarning::NativeConstruct() {
	KaijuCharacterEventSubsystem = this->GetWorld()->GetSubsystem<UKaijuCharacterEventSubsystem>();
	KaijuCharacterEventSubsystem->OnKaijuHealthChanged.AddUniqueDynamic(this, &UHealthWarning::CheckHealthChange);
	DeactivateWarning();
	Super::NativeConstruct();
}

void UHealthWarning::NativeDestruct()
{
	KaijuCharacterEventSubsystem->OnKaijuHealthChanged.RemoveDynamic(this, &UHealthWarning::CheckHealthChange);
	Super::NativeDestruct();
}

void UHealthWarning::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{

	Super::NativeTick(MyGeometry, InDeltaTime);
	//if (warningIsActive) {
		//Do something here
	//}
}

void UHealthWarning::CheckHealthChange(float OldHealth, float NewHealth, float MaxHealth)
{
	if (warningIsActive)
	{
		if (NewHealth > warningThreshold)
		{
			UHealthWarning::SetWarningActive(false);
		}
	}
	else
	{
		if (NewHealth <= warningThreshold)
		{
			UHealthWarning::SetWarningActive(true);
			UE_LOG(LogTemp, Warning, TEXT("Warning started with health %f at threshold %f"), NewHealth, warningThreshold);
		}
	}
}

void UHealthWarning::SetWarningActive(bool value)
{
	//Anything you want to do when it is activated or deactivated
	warningIsActive = value;
	if (warningIsActive == false) {
		DeactivateWarning();
	}
}

