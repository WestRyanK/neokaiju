// Fill out your copyright notice in the Description page of Project Settings.


#include "TagAwareLerpedSpringArmComponent.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"

float UTagAwareLerpedSpringArmComponent::GetLerpSpeed(ELerpedSpringArmMotion Motion)
{
	float LerpSpeed = Super::GetLerpSpeed(Motion);

	bool bCanExtend = true;
	bool bCanRetract = true;
	IAbilitySystemInterface* Interface = Cast<IAbilitySystemInterface>(this->GetOwner());
	if (Interface)
	{
		UAbilitySystemComponent* ASC = Interface->GetAbilitySystemComponent();
		if (IsValid(ASC))
		{
			bCanExtend = !ASC->HasAnyMatchingGameplayTags(this->ExtendBlockingParentTags);
			bCanRetract = !ASC->HasAnyMatchingGameplayTags(this->RetractBlockingParentTags);
		}
	}

	if ((Motion == ELerpedSpringArmMotion::Extend && !bCanExtend) ||
		(Motion == ELerpedSpringArmMotion::Retract && !bCanRetract))
	{
		return 0.0f;
	}
	else
	{
		return LerpSpeed;
	}
}
