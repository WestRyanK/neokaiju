// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AbilitySystemComponent.h"
#include "EffectUtility.generated.h"

class AActor;
/**
 *
 */
UCLASS()
class NEOKAIJU_API UEffectUtility : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
		// Returns true if effect was successfully applied
		static bool ApplyEffectToActor(AActor* Actor, TSubclassOf<UGameplayEffect> Effect);

	UFUNCTION(BlueprintCallable)
		static void ApplyEffectAtRadius(
			UObject* WorldContextObject,
			FVector Epicenter,
			float Radius,
			TSubclassOf<UGameplayEffect> EffectToApply,
			TSet<AActor*>& AlreadyHitActors,
			const TArray<AActor*>& ActorsToIgnore,
			bool bShouldDrawDebug = false);

	UFUNCTION(BlueprintCallable)
		static void ApplyEffectsAtRadii(
			UObject* WorldContextObject,
			FVector Epicenter,
			const TMap<float, TSubclassOf<UGameplayEffect>>& EffectsAtRadii,
			const TArray<AActor*>& ActorsToIgnore,
			bool bShouldDrawDebug = false);

	UFUNCTION(BlueprintCallable)
		static void DrawDebugRadiusSphere(UObject* WorldContextObject, FVector Epicenter, float Radius, bool bShouldDrawDebug);
};
