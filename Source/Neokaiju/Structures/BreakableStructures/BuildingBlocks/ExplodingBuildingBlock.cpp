// Fill out your copyright notice in the Description page of Project Settings.


#include "ExplodingBuildingBlock.h"
#include "Engine/World.h"
#include "Neokaiju/Ability/Core/EffectUtility.h"
#include "AbilitySystemInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Neokaiju/KaijuCharacter/KaijuCharacter.h"


void AExplodingBuildingBlock::DestroyStructure()
{
	//if (!this->bIsLoadingState && !this->bHasAlreadyExploded)
	//{
	//	this->SetIsSimulatingPhysics(false);
	//	float ExplodeDelay = FMath::FRandRange(this->ExplodeDelayMinSeconds, this->ExplodeDelayMaxSeconds);
	//	this->SetActorEnableCollision(false);
	//	this->Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//	this->OnAboutToExplode(ExplodeDelay);
	//	this->bHasAlreadyExploded = true;
	//	if (ExplodeDelay <= 0.0f)
	//	{
	//		this->Explode();
	//	}
	//	else
	//	{
	//		this->GetWorld()->GetTimerManager().SetTimer(this->ExplodeTimerHandle, this, &AExplodingBuildingBlock::Explode, ExplodeDelay, true);
	//	}
	//}
}

void AExplodingBuildingBlock::HealthChanged(const FOnAttributeChangeData& Data)
{
	if (Data.NewValue < Data.OldValue)
	{
		this->bHasBeenDamaged = true;
		AKaijuCharacter* Character = Cast<AKaijuCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
		float KaijuLevel = 0;
		if (IsValid(Character))
		{
			KaijuLevel = Character->GetKaijuLevel();
		}
		bool bwasThrown = this->GetAbilitySystemComponent()->HasMatchingGameplayTag(this->ThrownTag);

		if (KaijuLevel >= 4 || bwasThrown)
		{
			this->Explode();
		}
		else
		{
			float ExplodeDelay = FMath::FRandRange(this->ExplodeDelayMinSeconds, this->ExplodeDelayMaxSeconds);
			this->OnAboutToExplode(ExplodeDelay);
			if (ExplodeDelay <= 0.0f)
			{
				this->Explode();
			}
			else
			{
				this->GetWorld()->GetTimerManager().SetTimer(this->ExplodeTimerHandle, this, &AExplodingBuildingBlock::Explode, ExplodeDelay, true);
			}
		}
	}

	Super::HealthChanged(Data);
}


float AExplodingBuildingBlock::GetMaxExplodeRadius() const
{
	if (this->ExplodeEffectsAtRadii.Num() > 0)
	{
		TArray<float> Radii;
		this->ExplodeEffectsAtRadii.GetKeys(Radii);
		Radii.Sort();
		return Radii.Last();
	}
	else
	{
		return 0.0f;
	}
}

void AExplodingBuildingBlock::Explode()
{
	FVector Epicenter = this->Mesh->GetComponentLocation();
	this->OnExplode(Epicenter);

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	UEffectUtility::ApplyEffectsAtRadii(
		this,
		Epicenter,
		this->ExplodeEffectsAtRadii,
		ActorsToIgnore,
		this->bDrawDebugExplosions);

	Super::DestroyStructure();
}

void AExplodingBuildingBlock::OnMeshHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::OnMeshHit(HitComp, OtherActor, OtherComp, NormalImpulse, Hit);
}
