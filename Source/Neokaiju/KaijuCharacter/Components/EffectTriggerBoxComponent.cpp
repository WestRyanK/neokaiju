// Fill out your copyright notice in the Description page of Project Settings.


#include "EffectTriggerBoxComponent.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"

void UEffectTriggerBoxComponent::BeginPlay()
{
	Super::BeginPlay();

	this->OnComponentBeginOverlap.AddUniqueDynamic(this, &UEffectTriggerBoxComponent::BeginOverlapTrigger);
	this->OnComponentEndOverlap.AddUniqueDynamic(this, &UEffectTriggerBoxComponent::EndOverlapTrigger);
}

void UEffectTriggerBoxComponent::OnComponentDestroyed(bool bDestroyingHierarchy)
{
	this->OnComponentBeginOverlap.RemoveDynamic(this, &UEffectTriggerBoxComponent::BeginOverlapTrigger);
	this->OnComponentEndOverlap.RemoveDynamic(this, &UEffectTriggerBoxComponent::EndOverlapTrigger);
}

void UEffectTriggerBoxComponent::BeginOverlapTrigger(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IsValid(this->TriggeredEffect))
	{
		if (OtherActor != this->GetOwner())
		{
			UAbilitySystemComponent* OwnerASC = this->GetOwnerASC();
			if (IsValid(OwnerASC))
			{
				// If we apply the effect multiple times, it should return the same handle and get stacked on the OwnerASC? I think...
				// I think we can only stack duration/infinite effects...
				this->ActiveEffectHandle = OwnerASC->ApplyGameplayEffectToSelf(this->TriggeredEffect.GetDefaultObject(), 1, OwnerASC->MakeEffectContext());
				UE_LOG(LogTemp, Display, TEXT("Begin %s"), *OtherActor->GetFName().ToString());
			}
		}
	}

	//this->CurrentOverlapCount++;
}

void UEffectTriggerBoxComponent::EndOverlapTrigger(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//this->CurrentOverlapCount--;
	//if (this->CurrentOverlapCount == 0 && this->ActiveEffectHandle.IsValid())
	if (this->ActiveEffectHandle.IsValid())
	{
		if (OtherActor != this->GetOwner())
		{
			UAbilitySystemComponent* OwnerASC = this->GetOwnerASC();
			if (IsValid(OwnerASC))
			{
				OwnerASC->RemoveActiveGameplayEffect(this->ActiveEffectHandle, 1);
				UE_LOG(LogTemp, Display, TEXT("End %s"), *OtherActor->GetFName().ToString());
			}
		}
	}
}

UAbilitySystemComponent* UEffectTriggerBoxComponent::GetOwnerASC() const
{
	if (IsValid(this->GetOwner()))
	{
		IAbilitySystemInterface* ASCActor = Cast<IAbilitySystemInterface>(this->GetOwner());
		return ASCActor->GetAbilitySystemComponent();
	}
	return nullptr;
}
