// Fill out your copyright notice in the Description page of Project Settings.


#pragma once

#include "CoreMinimal.h"
#include "Neokaiju/Ability/Core/Tasks/MoveAlongCurveAbilityTask.h"
#include "DashAbilityTask.generated.h"

class UCurveFloat;
class AKaijuCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FDashAbilityTaskDelegate, FVector, CharacterVelocity, FVector, ImpactVelocity, AActor*, HitActor);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDashAbilityTaskTickDelegate, float, DeltaTime, FVector, DashDirection);
/**
 *
 */
UCLASS()
class NEOKAIJU_API UDashAbilityTask : public UMoveAlongCurveAbilityTask
{
	GENERATED_BODY()

public:

	// Static Constructor for UDashAbilityTask
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
		static UDashAbilityTask* Dash(
			UGameplayAbility* OwningAbility,
			FVector DashDirection,
			float DashDistance,
			float DashDuration,
			bool bDoesAllowGlancingHitRedirect,
			float MinGlancingHitRedirectAngle,
			bool bDoesAllowOffCenterHitSliding,
			float MaxOffCenterClipHitSize,
			UCurveFloat* DashPositionOverTime,
			bool bContinueVelocityAfterDash);

	UPROPERTY()
		bool bDoesAllowGlancingHitRedirect;

	UPROPERTY()
		float MinGlancingHitRedirectAngle;

	UPROPERTY()
		bool bDoesAllowOffCenterHitSliding;

	UPROPERTY()
		float MaxOffCenterClipHitSize;

	UPROPERTY()
		bool bContinueVelocityAfterDash;

	UPROPERTY(BlueprintAssignable)
		FDashAbilityTaskDelegate OnDashImpeded;

	UPROPERTY(BlueprintAssignable)
		FDashAbilityTaskDelegate OnDashHit;

	void DashImpeded(FVector CharacterVelocity, FVector ImpactNormal, AActor* HitActor);

	void DashHit(FVector CharacterVelocity, FVector ImpactNormal, AActor* HitActor);

	virtual void OnDestroy(bool bInOwnerFinished);

	virtual void Activate() override;

protected:
	FVector GetZXOnlyVector(FVector Vector) const;

	void OnHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit) override;

	bool RedirectDashIfGlancingHit(const FHitResult& Hit);

	bool SlideDashIfClippingHit(const FHitResult& Hit);

	bool bDebugIsFirstHit;

	float CapsuleRadius;

	float MaxImpactAngleValidForClipHit;

	void DrawDebugAngles(const FHitResult& Hit, FVector TangentInDashDirection, float ClipSize);
};
