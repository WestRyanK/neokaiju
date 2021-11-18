// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageableAttributeSet.h"


UDamageableAttributeSet::UDamageableAttributeSet()
{

}

// Handles CurrentValue (Temporary value) clamping
void UDamageableAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	this->ClampAttribute(Attribute, NewValue, this->GetHealthAttribute(), this->GetMaxHealthAttribute());
}

// Handles BaseValue (Permanent value) clamping
void UDamageableAttributeSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);

	this->ClampAttribute(Attribute, NewValue, this->GetHealthAttribute(), this->GetMaxHealthAttribute());
}

void UDamageableAttributeSet::ClampAttribute(const FGameplayAttribute& CurrentAttribute, float& ValueToClamp, FGameplayAttribute AttributeToClamp, FGameplayAttribute MaxAttribute) const
{
	if (this->bShouldClampAttributes)
	{
		if (CurrentAttribute == AttributeToClamp)
		{
			ValueToClamp = FMath::Clamp<float>(ValueToClamp, 0.0f, MaxAttribute.GetNumericValue(this));
		}
		if (CurrentAttribute == MaxAttribute && ValueToClamp < AttributeToClamp.GetNumericValue(this))
		{
			AttributeToClamp.SetNumericValueChecked(ValueToClamp, const_cast<UDamageableAttributeSet*>(this));
		}
	}
}
