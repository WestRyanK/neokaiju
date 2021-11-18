// Fill out your copyright notice in the Description page of Project Settings.


#include "ThrownAbility.h"
#include "AbilitySystemInterface.h"
#include "Neokaiju/Ability/Core/EffectUtility.h"
#include "Engine/World.h"


UThrownAbility::UThrownAbility()
{
	this->bDoesAbilityActivateOnGranted = true;
}

void UThrownAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!this->CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		this->EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AActor* Actor = this->GetAvatarActorFromActorInfo();
	if (IsValid(Actor))
	{
		Actor->OnActorHit.AddUniqueDynamic(this, &UThrownAbility::OnHit);
		this->SetThrownCollision();
	}
	else
	{
		this->EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
	}
}

void UThrownAbility::SetThrownCollision()
{
	AActor* Actor = this->GetAvatarActorFromActorInfo();
	if (IsValid(Actor))
	{
		TArray<UPrimitiveComponent*> Components;
		Actor->GetComponents(Components);
		for (UPrimitiveComponent* Component : Components)
		{
			Component->SetCollisionObjectType(this->ThrownCollisionChannel);
		}

		if (this->InitialNoCollisionPeriod > 0)
		{
			Actor->SetActorEnableCollision(false);
			this->GetWorld()->GetTimerManager().SetTimer(this->TimerHandle_EnableCollision, this, &UThrownAbility::EnableCollision, this->InitialNoCollisionPeriod, false);
		}
	}
}

void UThrownAbility::EnableCollision()
{
	AActor* Actor = this->GetAvatarActorFromActorInfo();
	if (IsValid(Actor))
	{
		Actor->SetActorEnableCollision(true);
	}
}

void UThrownAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	AActor* Actor = this->GetAvatarActorFromActorInfo();
	if (IsValid(Actor))
	{
		Actor->OnActorHit.RemoveAll(this);
	}

	this->GetWorld()->GetTimerManager().ClearTimer(this->TimerHandle_EnableCollision);
	this->EnableCollision();

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UThrownAbility::OnHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
	UEffectUtility::ApplyEffectToActor(SelfActor, this->ImpactEffect);
	UEffectUtility::ApplyEffectToActor(OtherActor, this->ImpactEffect);

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(SelfActor);
	ActorsToIgnore.Add(OtherActor);

	//FVector ImpactDirection = -Hit.ImpactNormal;
	FVector ImpactDirection = -NormalImpulse.GetSafeNormal();
	//FVector ImpactDirection = (Hit.ImpactPoint - SelfActor->GetActorLocation()).GetSafeNormal();

	FVector Epicenter = Hit.ImpactPoint + ImpactDirection * this->SplashDamageOffset;
	//FVector Epicenter = Hit.ImpactPoint - Hit.ImpactNormal * this->SplashDamageOffset;
	UEffectUtility::ApplyEffectsAtRadii(this, Epicenter, this->SplashDamageEffects, ActorsToIgnore, this->bIsAbilityDebugEnabled);

	this->EndAbility(this->CurrentSpecHandle, this->CurrentActorInfo, this->CurrentActivationInfo, true, false);
}
