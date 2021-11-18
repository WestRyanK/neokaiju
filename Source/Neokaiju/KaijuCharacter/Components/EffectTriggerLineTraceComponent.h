// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SplineComponent.h"
#include "AbilitySystemComponent.h"
#include "EffectTriggerLineTraceComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class NEOKAIJU_API UEffectTriggerLineTraceComponent : public USplineComponent
{
	GENERATED_BODY()

public:

	UEffectTriggerLineTraceComponent();

	UEffectTriggerLineTraceComponent(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(BlueprintReadOnly)
		FHitResult CurrentTraceHitResult;

	UPROPERTY(BlueprintReadOnly)
		bool bDidCurrentTraceHit = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TSubclassOf<UGameplayEffect> TriggeredEffect;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float CollisionDepthDistance;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float CollisionHeight = 10.0f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TEnumAsByte<ECollisionChannel> TriggerTraceChannel;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		bool bShouldDrawTrace = false;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	FActiveGameplayEffectHandle ActiveEffectHandle;

	UAbilitySystemComponent* GetOwnerASC() const;

	
};
