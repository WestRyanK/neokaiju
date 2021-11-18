// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "Neokaiju/Ability/Core/Tasks/WaitForSpecificInputAbilityTask.h"
#include "TimerManager.h"
#include "FireLaserAbilityTask.generated.h"

class UParticleSystemComponent;
class AKaijuCharacter;
class UAudioComponent;
class USceneComponent;
class USkeletalMeshSocket;
class USkeletalMeshComponent;


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FFireLaserAbilityTaskDelegate, FVector, LaserAimDirection, FHitResult, LaserHit);
/**
 *
 */
UCLASS()
class NEOKAIJU_API UFireLaserAbilityTask : public UWaitForSpecificInputAbilityTask
{
	GENERATED_BODY()

public:
	UFireLaserAbilityTask();

	// Static Constructor for UDashAbilityTask
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
		static UFireLaserAbilityTask* FireLaser(
			UGameplayAbility* OwningAbility,
			TSubclassOf<UGameplayEffect> LaserDamageEffect,
			TSubclassOf<UGameplayEffect> LaserOverlapEffect,
			float LaserDamageRate,
			float LaserCommitRate,
			float LaserAimSpeed,
			float LaserExtendSpeed,
			float MaxLaserDistance,
			FName SocketName,
			USkeletalMeshComponent* Skeleton,
			TEnumAsByte<ECollisionChannel> LaserTraceChannel,
			float LaserRadius,
			float LaserImpactRadius,
			float LaserDepth);

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
		FName SocketName;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
		USkeletalMeshComponent* Skeleton;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
		float LaserRadius;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
		float LaserDepth;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
		float LaserImpactRadius;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
		float LaserAimSpeed;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
		float LaserExtendSpeed;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
		float MaxLaserDistance;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
		float CostCommitRate;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
		class TSubclassOf<class UGameplayEffect> LaserDamageEffect;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
		class TSubclassOf<class UGameplayEffect> LaserOverlapEffect;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
		float LaserDamageRate;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
		TEnumAsByte<ECollisionChannel> LaserTraceChannel;

	UPROPERTY(BlueprintAssignable)
		FFireLaserAbilityTaskDelegate OnFireLaserComplete;

	UPROPERTY(BlueprintAssignable)
		FFireLaserAbilityTaskDelegate OnFireLaserTick;

	virtual void Activate() override;

	virtual void TickTask(float DeltaTime) override;

	virtual void OnReleaseNoticed();

	virtual void OnDestroy(bool bInOwnerFinished) override;

	void Finish();
private:
	float CurrentLaserDistance;

	void UpdateLaserAimDirection(float DeltaTime);

	bool TraceLaser(FHitResult& OutHitResult) const;

	void UpdateLaserDistance(float DeltaTime, bool bShouldOverrideDistance, float OverrideDistance);

	FVector LaserAimDirection;

	AKaijuCharacter* Character;

	FTimerHandle CostCommitTimerHandle;

	FTimerHandle DamageActorTimerHandle;

	void HitActorsAtEndOfLaser(FVector EndOfLaser);

	void CommitCostEvent();

	void DamageHitActors();

	void AffectOverlappedActors(const FHitResult& HitResult);

	TArray<AActor*> LastHitActors;

	TArray<AActor*> LastOverlapActors;

	float TimeOfLastLaserDamage = 0;
};
