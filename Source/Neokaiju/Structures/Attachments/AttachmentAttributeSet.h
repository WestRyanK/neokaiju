// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Neokaiju/Core/DamageableAttributeSet.h"
#include "AttachmentAttributeSet.generated.h"

/**
 * 
 */
UCLASS()
class NEOKAIJU_API UAttachmentAttributeSet : public UDamageableAttributeSet
{
	GENERATED_BODY()

public:
	UAttachmentAttributeSet();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
		FGameplayAttributeData MaximumKaijuLevelToEffect;
	ATTRIBUTE_ACCESSORS(UAttachmentAttributeSet, MaximumKaijuLevelToEffect)
	
};
