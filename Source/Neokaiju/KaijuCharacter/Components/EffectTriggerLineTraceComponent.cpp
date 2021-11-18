// Fill out your copyright notice in the Description page of Project Settings.


#include "EffectTriggerLineTraceComponent.h"
#include "AbilitySystemInterface.h"
#include "Neokaiju/Core/TraceUtility.h"


UEffectTriggerLineTraceComponent::UEffectTriggerLineTraceComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = true;
}

UEffectTriggerLineTraceComponent::UEffectTriggerLineTraceComponent() : Super()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UEffectTriggerLineTraceComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{

	FVector StartLocation = this->GetLocationAtSplineInputKey(0.0f, ESplineCoordinateSpace::World);
	FVector EndLocation = this->GetLocationAtSplineInputKey(1.0f, ESplineCoordinateSpace::World);

	FHitResult NewTraceHitResult;

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this->GetOwner());
	bool bDidNewTraceHit = UTraceUtility::BoxTrace(this, NewTraceHitResult, StartLocation, EndLocation, this->CollisionHeight, this->CollisionDepthDistance, this->TriggerTraceChannel, QueryParams, this->bShouldDrawTrace);

	bool bDidBeginHitting = (this->bDidCurrentTraceHit == false && bDidNewTraceHit == true);
	bool bDidEndHitting = (this->bDidCurrentTraceHit == true && bDidNewTraceHit == false);

	this->CurrentTraceHitResult = NewTraceHitResult;
	this->bDidCurrentTraceHit = bDidNewTraceHit;

	if (bDidBeginHitting)
	{
		if (IsValid(this->TriggeredEffect))
		{
			UAbilitySystemComponent* OwnerASC = this->GetOwnerASC();
			if (IsValid(OwnerASC))
			{
				// If we apply the effect multiple times, it should return the same handle and get stacked on the OwnerASC? I think...
				// I think we can only stack duration/infinite effects...
				this->ActiveEffectHandle = OwnerASC->ApplyGameplayEffectToSelf(this->TriggeredEffect.GetDefaultObject(), 1, OwnerASC->MakeEffectContext());
			}
		}
	}
	else if (bDidEndHitting)
	{
		if (this->ActiveEffectHandle.IsValid())
		{
			UAbilitySystemComponent* OwnerASC = this->GetOwnerASC();
			if (IsValid(OwnerASC))
			{
				OwnerASC->RemoveActiveGameplayEffect(this->ActiveEffectHandle, 1);
			}
		}
	}
}


UAbilitySystemComponent* UEffectTriggerLineTraceComponent::GetOwnerASC() const
{
	if (IsValid(this->GetOwner()))
	{
		IAbilitySystemInterface* ASCActor = Cast<IAbilitySystemInterface>(this->GetOwner());
		return ASCActor->GetAbilitySystemComponent();
	}
	return nullptr;
}
