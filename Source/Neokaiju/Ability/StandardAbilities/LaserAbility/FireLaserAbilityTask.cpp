// Fill out your copyright notice in the Description page of Project Settings.


#include "FireLaserAbilityTask.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Neokaiju/Core/SafeDirectionUtility.h"
#include "Neokaiju/Core/TraceUtility.h"
#include "Neokaiju/KaijuCharacter/KaijuCharacter.h"
#include "Neokaiju/Ability/Core/Abilities/BaseGameplayAbility.h"
#include "Components/SceneComponent.h"
#include "Neokaiju/KaijuCharacter/Components/KaijuMovementComponent.h"
#include "Components/AudioComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Engine/SkeletalMeshSocket.h"


UFireLaserAbilityTask::UFireLaserAbilityTask()
{
	this->bTickingTask = true;
}

UFireLaserAbilityTask* UFireLaserAbilityTask::FireLaser(
	UGameplayAbility* OwningAbility,
	TSubclassOf<UGameplayEffect> LaserDamageEffect,
	TSubclassOf<UGameplayEffect> LaserOverlapEffect,
	float LaserDamageRate,
	float CostCommitRate,
	float LaserAimSpeed,
	float LaserExtendSpeed,
	float MaxLaserDistance,
	FName SocketName,
	USkeletalMeshComponent* Skeleton,
	TEnumAsByte<ECollisionChannel> LaserTraceChannel,
	float LaserRadius,
	float LaserImpactRadius,
	float LaserDepth)
{
	UFireLaserAbilityTask* FireLaserAbilityTask = NewAbilityTask<UFireLaserAbilityTask>(OwningAbility);
	FireLaserAbilityTask->LaserDamageEffect = LaserDamageEffect;
	FireLaserAbilityTask->LaserOverlapEffect = LaserOverlapEffect;
	FireLaserAbilityTask->LaserDamageRate = LaserDamageRate;
	FireLaserAbilityTask->CostCommitRate = CostCommitRate;
	FireLaserAbilityTask->SocketName = SocketName;
	FireLaserAbilityTask->LaserAimSpeed = LaserAimSpeed;
	FireLaserAbilityTask->LaserExtendSpeed = LaserExtendSpeed;
	FireLaserAbilityTask->MaxLaserDistance = MaxLaserDistance;
	FireLaserAbilityTask->Skeleton = Skeleton;
	FireLaserAbilityTask->LaserTraceChannel = LaserTraceChannel;
	FireLaserAbilityTask->LaserRadius = LaserRadius;
	FireLaserAbilityTask->LaserImpactRadius = LaserImpactRadius;
	FireLaserAbilityTask->LaserDepth = LaserDepth;

	return FireLaserAbilityTask;
}

void UFireLaserAbilityTask::Activate()
{
	Super::Activate();

	this->Character = Cast<AKaijuCharacter>(this->GetAvatarActor());
	this->LaserAimDirection = this->Character->GetPrimaryAimDirection();
	this->CurrentLaserDistance = 0;

	this->GetWorld()->GetTimerManager().SetTimer(this->CostCommitTimerHandle, this, &UFireLaserAbilityTask::CommitCostEvent, this->CostCommitRate, true);
	this->GetWorld()->GetTimerManager().SetTimer(this->DamageActorTimerHandle, this, &UFireLaserAbilityTask::DamageHitActors, this->LaserDamageRate, true);
}

bool UFireLaserAbilityTask::TraceLaser(FHitResult& OutHitResult) const
{
	FVector LaserStartPoint = this->Skeleton->GetSocketLocation(SocketName);
	FVector LaserEndPoint = this->LaserAimDirection * this->CurrentLaserDistance + LaserStartPoint;

	UBaseGameplayAbility* BaseAbility = Cast<UBaseGameplayAbility>(this->Ability);

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this->Character);
	bool bDidHitOccur = UTraceUtility::BoxTrace(
		this,
		OutHitResult,
		LaserStartPoint,
		LaserEndPoint,
		this->LaserRadius,
		this->LaserDepth,
		this->LaserTraceChannel,
		QueryParams,
		BaseAbility->bIsAbilityDebugEnabled);

	if (!bDidHitOccur)
	{
		OutHitResult.Actor = nullptr;
		OutHitResult.ImpactPoint = LaserEndPoint;
		OutHitResult.Distance = this->CurrentLaserDistance;
	}
	else
	{
		OutHitResult.ImpactPoint.Y = LaserEndPoint.Y;
	}
	return bDidHitOccur;
}

void UFireLaserAbilityTask::Finish()
{
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		this->OnFireLaserComplete.Broadcast(FVector::ZeroVector, FHitResult());
	}

	this->EndTask();
}


void UFireLaserAbilityTask::TickTask(float DeltaTime)
{
	Super::TickTask(DeltaTime);

	FHitResult HitResult;
	bool bDidHit = this->TraceLaser(HitResult);

	this->UpdateLaserAimDirection(DeltaTime);
	// We want to extend the laser just a little bit further so that when it collides with an object, 
	// the end point will be far enough along that it will still collide in the next Tick's TraceLaser.
	// If we don't add this amount, then next tick, the trace won't be long enough to touch the same object it just hit.
	const float SMALL_DISTANCE = 10;
	this->UpdateLaserDistance(DeltaTime, bDidHit, HitResult.Distance + SMALL_DISTANCE);

	this->HitActorsAtEndOfLaser(HitResult.ImpactPoint);
 	this->AffectOverlappedActors(HitResult);

	if (this->ShouldBroadcastAbilityTaskDelegates())
	{
		this->OnFireLaserTick.Broadcast(this->LaserAimDirection, HitResult);
	}
}

void UFireLaserAbilityTask::HitActorsAtEndOfLaser(FVector EndOfLaser)
{
	this->LastHitActors.Empty();
	FHitResult HitResult;

	UBaseGameplayAbility* BaseAbility = Cast<UBaseGameplayAbility>(this->Ability);
	FVector LaserImpactStart = EndOfLaser;
	FVector LaserImpactEnd = EndOfLaser + LaserAimDirection * this->LaserImpactRadius;
	FCollisionQueryParams QueryParams;
	bool bDidHitOccur = true;
	while (bDidHitOccur)
	{
		bDidHitOccur = UTraceUtility::BoxTrace(
			this,
			HitResult,
			LaserImpactStart,
			LaserImpactEnd,
			this->LaserRadius,
			this->LaserDepth,
			this->LaserTraceChannel,
			QueryParams,
			BaseAbility->bIsAbilityDebugEnabled);

		if (bDidHitOccur)
		{
			if (!this->LastHitActors.Contains(HitResult.Actor.Get()))
			{
				this->LastHitActors.Add(HitResult.Actor.Get());
				QueryParams.AddIgnoredActor(HitResult.Actor.Get());
			}
		}
	}

	UE_LOG(LogTemp, Display, TEXT("Hit actors: %d"), this->LastHitActors.Num());
}


void UFireLaserAbilityTask::DamageHitActors()
{
	for (AActor* HitActor : this->LastHitActors)
	{
		IAbilitySystemInterface* ASCActor = Cast<IAbilitySystemInterface>(HitActor);
		if (ASCActor)
		{
			if (IsValid(this->LaserDamageEffect))
			{
				// To determine the amount of damage to do in order to destroy a block in a certain amount of time, use this equation:
				// DamageAmount = BlockHealth * LaserDamageRate / TimeToDestroy
				this->AbilitySystemComponent->ApplyGameplayEffectToTarget(this->LaserDamageEffect.GetDefaultObject(), ASCActor->GetAbilitySystemComponent(), this->Character->GetKaijuLevel());
			}
		}
	}
}

void UFireLaserAbilityTask::AffectOverlappedActors(const FHitResult& HitResult) {
	TArray<FHitResult> outResult;
	FCollisionQueryParams QueryParams;
	bool bDidHitOccur = true;
	UBaseGameplayAbility* BaseAbility = Cast<UBaseGameplayAbility>(this->Ability);
	
	bDidHitOccur = UTraceUtility::BoxMultiTrace(
		this,
		outResult,
		HitResult.TraceStart,
		HitResult.TraceEnd,
		this->LaserRadius,
		this->LaserDepth,
		this->LaserTraceChannel,
		QueryParams,
		BaseAbility->bIsAbilityDebugEnabled);

	this->LastOverlapActors.Empty();

	for (FHitResult result : outResult) {
		if (!result.bBlockingHit) {
			this->LastOverlapActors.Add(result.GetActor());
		}
	}

	for (AActor* overlappingActor : LastOverlapActors) {
		IAbilitySystemInterface* ASCActor = Cast<IAbilitySystemInterface>(overlappingActor);
		if (ASCActor)
		{
			if (IsValid(this->LaserOverlapEffect))
			{
				// To determine the amount of damage to do in order to destroy a block in a certain amount of time, use this equation:
				// DamageAmount = BlockHealth * LaserDamageRate / TimeToDestroy
				this->AbilitySystemComponent->ApplyGameplayEffectToTarget(this->LaserOverlapEffect.GetDefaultObject(), ASCActor->GetAbilitySystemComponent(), this->Character->GetKaijuLevel());
			}
		}
	}



}

void UFireLaserAbilityTask::UpdateLaserAimDirection(float DeltaTime)
{
	if (this->Character->GetIsPlayerAiming())
	{
		FVector ForwardDirection = USafeDirectionUtility::GetSafeForwardDirectionFromFacing(this->Character->KaijuMovement->GetFacingDirection());
		this->LaserAimDirection = USafeDirectionUtility::RotateVectorOnYAxisTowards(
			this->LaserAimDirection,
			this->Character->GetPrimaryAimDirection(),
			ForwardDirection,
			FVector::UpVector,
			this->LaserAimSpeed,
			DeltaTime);
	}
}

void UFireLaserAbilityTask::UpdateLaserDistance(float DeltaTime, bool bShouldOverrideDistance, float OverrideDistance)
{
	//if (bShouldOverrideDistance)
	if (false)
	{
		this->CurrentLaserDistance = OverrideDistance;
	}
	else
	{
		if (this->LaserExtendSpeed > 0.0f)
		{
			this->CurrentLaserDistance = FMath::Min(this->CurrentLaserDistance + this->LaserExtendSpeed * DeltaTime, this->MaxLaserDistance);
		}
		else
		{
			this->CurrentLaserDistance = this->MaxLaserDistance;
		}
	}
}


void UFireLaserAbilityTask::CommitCostEvent()
{
	if (!this->Ability->CommitAbilityCost(this->Ability->GetCurrentAbilitySpecHandle(), this->Ability->GetCurrentActorInfo(), this->Ability->GetCurrentActivationInfo()))
	{
		this->Finish();
	}
}

void UFireLaserAbilityTask::OnReleaseNoticed()
{
	this->Finish();
	Super::OnReleaseNoticed();
}

void UFireLaserAbilityTask::OnDestroy(bool bInOwnerFinished)
{
	this->GetWorld()->GetTimerManager().ClearTimer(this->CostCommitTimerHandle);
	this->GetWorld()->GetTimerManager().ClearTimer(this->DamageActorTimerHandle);

	Super::OnDestroy(bInOwnerFinished);
}
