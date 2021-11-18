// Fill out your copyright notice in the Description page of Project Settings.


#include "KaijuAttributeSet.h"


UKaijuAttributeSet::UKaijuAttributeSet() 
{

}

// Handles CurrentValue (Temporary value) clamping
void UKaijuAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	this->ClampAttribute(Attribute, NewValue, this->GetLaserChargeAttribute(), this->GetMaxLaserChargeAttribute());
}

// Handles BaseValue (Permanent value) clamping
void UKaijuAttributeSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);

	this->ClampAttribute(Attribute, NewValue, this->GetLaserChargeAttribute(), this->GetMaxLaserChargeAttribute());
}
