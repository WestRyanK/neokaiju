// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupTW.h"
#include "Neokaiju/Pickups/PickupBase.h"
#include "Neokaiju/Pickups/PickupSubsystem.h"
#include "Engine/World.h"

void UPickupTW::Init(UWorld* WorldValue)
{
	UBaseTagWatcher::Init(WorldValue);

	UPickupSubsystem* PickupSubsystem = WorldValue->GetSubsystem<UPickupSubsystem>();
	PickupSubsystem->OnPickupPickedUp.AddUniqueDynamic(this, &UPickupTW::PickupPickedUp);
}

void UPickupTW::GetSubsystemDependencies(TSet<TSubclassOf<USubsystem>>& OutDependencies) const
{
	OutDependencies.Add(UPickupSubsystem::StaticClass());
}


void UPickupTW::PickupPickedUp(APickupBase* PickupPickedUp)
{
	if (IsValid(PickupPickedUp))
	{
		for (FName Tag : PickupPickedUp->Tags)
		{
			this->TagSeen(Tag);
		}
	}
}
