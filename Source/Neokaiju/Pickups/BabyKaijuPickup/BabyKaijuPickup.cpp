// Fill out your copyright notice in the Description page of Project Settings.


#include "BabyKaijuPickup.h"
#include "Engine/World.h"
#include "Neokaiju/KaijuCharacter/KaijuCharacterEventSubsystem.h"
#include "Neokaiju/Enemies/Swarm/SwarmEnemy.h"
#include "Neokaiju/Enemies/SwarmV2/SwarmV2.h"
#include "Components/SphereComponent.h"

ABabyKaijuPickup::ABabyKaijuPickup()
{
	this->DestroyActorsVolume = CreateDefaultSubobject<USphereComponent>("DestroyActorsVolume");
	this->DestroyActorsVolume->SetupAttachment(this->RootComponent);
}

void ABabyKaijuPickup::PickUp(AKaijuCharacter* Character)
{
	this->DestroyActorsInVolume();
	this->NotifyBabyKaijuSaved();
	if (IsValid(this->DestroyTriggerOnPickup))
	{
		this->DestroyTriggerOnPickup->Destroy();
	}

	Super::PickUp(Character);
}

void ABabyKaijuPickup::DestroyActorsInVolume()
{
	TSet<AActor*> OverlappingActors;

	this->DestroyActorsVolume->GetOverlappingActors(OverlappingActors, ASwarmV2::StaticClass());
	for (AActor* OverlappingActor : OverlappingActors)
	{
		// This is a pain point. Swarm should implement the DamageableAttributeSet so that we can simply set its health to zero and not worry about specifically SwarmEnemy, SwarmV2, etc.
		// Loose coupling!!!
		ASwarmV2* SwarmV2 = Cast<ASwarmV2>(OverlappingActor);
		if (IsValid(SwarmV2)) {
			SwarmV2->KillSwarm();
		}
	}
}

void ABabyKaijuPickup::NotifyBabyKaijuSaved()
{
	UKaijuCharacterEventSubsystem* Subsystem = this->GetWorld()->GetSubsystem<UKaijuCharacterEventSubsystem>();
	Subsystem->BabyKaijuSaved();
}
