// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Neokaiju/Ability/Core/Tasks/MoveAlongCurveAbilityTask.h"
#include "JumpAlongCurveAbilityTask.generated.h"

/**
 * 
 */
UCLASS()
class NEOKAIJU_API UJumpAlongCurveAbilityTask : public UMoveAlongCurveAbilityTask
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
		static UJumpAlongCurveAbilityTask* JumpAlongCurve(
			UGameplayAbility* OwningAbility,
			FVector JumpDirection,
			float JumpDistance,
			float JumpDuration,
			UCurveFloat* JumpPositionOverTime);

protected:
		virtual void OnHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit) override;
};
