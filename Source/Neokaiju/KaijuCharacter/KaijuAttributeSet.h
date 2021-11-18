// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "Neokaiju/Core/DamageableAttributeSet.h"
#include "KaijuAttributeSet.generated.h"

/**
 *	An AttributeSet containing all of the relevant attributes for the KaijuCharacter
 */
UCLASS()
class NEOKAIJU_API UKaijuAttributeSet : public UDamageableAttributeSet
{
	GENERATED_BODY()

public:
	UKaijuAttributeSet();

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
		FGameplayAttributeData KaijuLevel;
	ATTRIBUTE_ACCESSORS(UKaijuAttributeSet, KaijuLevel)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
		FGameplayAttributeData LaserDamage;
	ATTRIBUTE_ACCESSORS(UKaijuAttributeSet, LaserDamage)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
		FGameplayAttributeData LaserCharge;
	ATTRIBUTE_ACCESSORS(UKaijuAttributeSet, LaserCharge)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
		FGameplayAttributeData MaxLaserCharge;
	ATTRIBUTE_ACCESSORS(UKaijuAttributeSet, MaxLaserCharge)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
		FGameplayAttributeData DashDamage;
	ATTRIBUTE_ACCESSORS(UKaijuAttributeSet, DashDamage)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
		FGameplayAttributeData DashDistance;
	ATTRIBUTE_ACCESSORS(UKaijuAttributeSet, DashDistance)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
		FGameplayAttributeData DashDuration;
	ATTRIBUTE_ACCESSORS(UKaijuAttributeSet, DashDuration)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
		FGameplayAttributeData JumpHeight;
	ATTRIBUTE_ACCESSORS(UKaijuAttributeSet, JumpHeight)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
		FGameplayAttributeData WalkSpeed;
	ATTRIBUTE_ACCESSORS(UKaijuAttributeSet, WalkSpeed)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
		FGameplayAttributeData KaijuSizeScale;
	ATTRIBUTE_ACCESSORS(UKaijuAttributeSet, KaijuSizeScale)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
		FGameplayAttributeData LaserLength;
	ATTRIBUTE_ACCESSORS(UKaijuAttributeSet, LaserLength)
};
