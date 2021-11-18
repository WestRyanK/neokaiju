// Fill out your copyright notice in the Description page of Project Settings.


#include "SwarmEnemy.h"
#include "AbilitySystemComponent.h"

// Sets default values
ASwarmEnemy::ASwarmEnemy()
{
	this->AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
}

// Called when the game starts or when spawned
void ASwarmEnemy::BeginPlay()
{
	Super::BeginPlay();

	this->ListenForTags();
}

UAbilitySystemComponent* ASwarmEnemy::GetAbilitySystemComponent() const
{
	return this->AbilitySystemComponent;
}

void ASwarmEnemy::ListenForTags()
{
	this->AbilitySystemComponent->RegisterGameplayTagEvent(this->HitByLaserTag, EGameplayTagEventType::NewOrRemoved)
		.AddUObject(this, &ASwarmEnemy::HitByLaserTagChanged);
}

void ASwarmEnemy::HitByLaserTagChanged(FGameplayTag Tag, int32 Count)
{
	if (Count == 1)
	{
		this->BeginHitByLaser();
	}
	else if (Count == 0)
	{
		this->EndHitByLaser();
	}
}
