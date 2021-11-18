// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"
#include "GameplayEffectExtension.h"
#include "DamageableAttributeSet.generated.h"

// Use ATTRIBUTE_ACCESSORS to quickly and easily define getters and setters for each attribute.
// https://github.com/tranek/GASDocumentation#443-defining-attributes
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
     GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
     GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
     GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
     GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * 
 */
UCLASS()
class NEOKAIJU_API UDamageableAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UDamageableAttributeSet();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool bShouldClampAttributes = true;

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
		FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UDamageableAttributeSet, Health)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
		FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UDamageableAttributeSet, MaxHealth)
	
protected:
	void ClampAttribute(const FGameplayAttribute& CurrentAttribute, float& ValueToClamp, FGameplayAttribute AttributeToClamp, FGameplayAttribute MaxAttribute) const;
};
