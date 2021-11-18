// Fill out your copyright notice in the Description page of Project Settings.


#include "LerpUtility.h"
#include "Engine/World.h"

ULerpUtility* ULerpUtility::LerpVectorOverTime(UObject* WorldContextObject, UObject* Outer, FVector Start, FVector End, float Duration, float TickInterval, float InitialDelay)
{
	ULerpUtility* LerpUtility = ULerpUtility::CreateLerpUtility(WorldContextObject, Outer, Duration, TickInterval, InitialDelay);
	LerpUtility->StartVector = Start;
	LerpUtility->EndVector = End;

	return LerpUtility;
}

ULerpUtility* ULerpUtility::LerpFloatOverTime(UObject* WorldContextObject, UObject* Outer, float Start, float End, float Duration, float TickInterval, float InitialDelay)
{
	ULerpUtility* LerpUtility = ULerpUtility::CreateLerpUtility(WorldContextObject, Outer, Duration, TickInterval, InitialDelay);
	LerpUtility->StartFloat = Start;
	LerpUtility->EndFloat = End;

	return LerpUtility;
}

ULerpUtility* ULerpUtility::CreateLerpUtility(UObject* WorldContextObject, UObject* Outer, float Duration, float TickInterval, float InitialDelay)
{
	ULerpUtility* LerpUtility = NewObject<ULerpUtility>(Outer);
	LerpUtility->Duration = Duration;
	LerpUtility->TickInterval = TickInterval;
	LerpUtility->WorldContextObject = WorldContextObject;
	LerpUtility->InitialDelay = InitialDelay;

	return LerpUtility;
}

void ULerpUtility::Begin()
{
	if (this->bIsTransitioning)
	{
		this->GetWorld()->GetTimerManager().ClearTimer(this->TimerHandle);
	}

	this->bIsTransitioning = true;

	if (this->InitialDelay <= 0)
	{
		this->BeginLerpTransition();
	}
	else
	{
		this->GetWorld()->GetTimerManager().SetTimer(this->TimerHandle, this, &ULerpUtility::BeginLerpTransition, this->InitialDelay);
	}
}

void ULerpUtility::End(ELerpSetValue SetValue)
{
	this->bIsTransitioning = false;
	this->WorldContextObject->GetWorld()->GetTimerManager().ClearTimer(this->TimerHandle);
	switch (SetValue)
	{
	case ELerpSetValue::StartValue:
		this->CurrentVector = this->StartVector;
		this->CurrentFloat = this->StartFloat;
		break;
	case ELerpSetValue::EndValue:
		this->CurrentVector = this->EndVector;
		this->CurrentFloat = this->EndFloat;
		break;
	case ELerpSetValue::CurrentValue:
	default:
		break;
	}

	this->FloatUpdated.Broadcast(this->CurrentFloat);
	this->VectorUpdated.Broadcast(this->CurrentVector);
	this->Completed.Broadcast();
}

void ULerpUtility::BeginLerpTransition()
{
	this->Alpha = 0.0f;

	if (this->Duration <= 0 || this->TickInterval <= 0)
	{
		this->AlphaIncrement = 1.0f;
		this->LerpUpdate();
	}
	else
	{
		this->AlphaIncrement = this->TickInterval / this->Duration ;
		this->WorldContextObject->GetWorld()->GetTimerManager().SetTimer(this->TimerHandle, this, &ULerpUtility::LerpUpdate, this->TickInterval, true);
	}
}

void ULerpUtility::LerpUpdate()
{
	this->Alpha += this->AlphaIncrement;

	this->Alpha = FMath::Clamp(this->Alpha, 0.0f, 1.0f);

	this->CurrentFloat = FMath::Lerp(this->StartFloat, this->EndFloat, this->Alpha);
	this->FloatUpdated.Broadcast(this->CurrentFloat);
	this->CurrentVector = FMath::Lerp(this->StartVector, this->EndVector, this->Alpha);
	this->VectorUpdated.Broadcast(this->CurrentVector);

	if (this->Alpha >= 1.0f)
	{
		this->End(ELerpSetValue::EndValue);
	}
}
