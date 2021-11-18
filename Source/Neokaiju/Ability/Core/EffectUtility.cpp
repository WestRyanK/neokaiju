// Fill out your copyright notice in the Description page of Project Settings.


#include "EffectUtility.h"
#include "GameFramework/Actor.h"
#include "DrawDebugHelpers.h"
#include "AbilitySystemInterface.h"

bool UEffectUtility::ApplyEffectToActor(AActor* Actor, TSubclassOf<UGameplayEffect> Effect)
{
	IAbilitySystemInterface* ASCInterface = Cast<IAbilitySystemInterface>(Actor);
	if (ASCInterface)
	{
		UAbilitySystemComponent* ASC = ASCInterface->GetAbilitySystemComponent();

		FActiveGameplayEffectHandle Handle = ASC->ApplyGameplayEffectToSelf(Effect.GetDefaultObject(), 1, ASC->MakeEffectContext());
		return Handle.WasSuccessfullyApplied();
	}
	return false;
}

void UEffectUtility::ApplyEffectAtRadius(
	UObject* WorldContextObject,
	FVector Epicenter,
	float Radius,
	TSubclassOf<UGameplayEffect> EffectToApply,
	TSet<AActor*>& AlreadyHitActors,
	const TArray<AActor*>& ActorsToIgnore,
	bool bShouldDrawDebug)
{
	if (IsValid(EffectToApply))
	{
		UEffectUtility::DrawDebugRadiusSphere(WorldContextObject, Epicenter, Radius, bShouldDrawDebug);

		FCollisionQueryParams Params;
		Params.AddIgnoredActors(ActorsToIgnore);
		TArray<FOverlapResult> Overlaps = TArray<FOverlapResult>();
		WorldContextObject->GetWorld()->OverlapMultiByObjectType(Overlaps, Epicenter, FQuat::Identity, FCollisionObjectQueryParams(), FCollisionShape::MakeSphere(Radius), Params);
		for (FOverlapResult Overlap : Overlaps)
		{
			AActor* HitActor = Overlap.GetActor();
			if (!AlreadyHitActors.Contains(HitActor) && IsValid(HitActor))
			{
				bool bWasEffectSuccessfullyApplied = UEffectUtility::ApplyEffectToActor(HitActor, EffectToApply);
				if (bWasEffectSuccessfullyApplied)
				{
					AlreadyHitActors.Add(HitActor);
				}
			}
		}
	}
}

void UEffectUtility::ApplyEffectsAtRadii(
	UObject* WorldContextObject,
	FVector Epicenter,
	const TMap<float, TSubclassOf<UGameplayEffect>>& EffectsAtRadii,
	const TArray<AActor*>& ActorsToIgnore,
	bool bShouldDrawDebug)
{
	TSet<AActor*> AlreadyHitActors;

	TArray<float> Radii;
	EffectsAtRadii.GetKeys(Radii);
	Radii.Sort();

	for (float Radius : Radii)
	{
		TSubclassOf<UGameplayEffect> EffectToApply = EffectsAtRadii[Radius];
		UEffectUtility::ApplyEffectAtRadius(WorldContextObject, Epicenter, Radius, EffectToApply, AlreadyHitActors, ActorsToIgnore, bShouldDrawDebug);
	}
}

void UEffectUtility::DrawDebugRadiusSphere(UObject* WorldContextObject, FVector Epicenter, float Radius, bool bShouldDrawDebug)
{
#if !UE_BUILD_SHIPPING // Don't draw trace when shipping
	if (bShouldDrawDebug)
	{
		UWorld* World = WorldContextObject->GetWorld();
		if (IsValid(World))
		{

			Epicenter.Y = 300;
			DrawDebugCircle(World, Epicenter, Radius, 64, FColor::Red, false, 5.0f, 0, 1.0f, FVector::ForwardVector, FVector::UpVector, false);
		}
	}
#endif
}
