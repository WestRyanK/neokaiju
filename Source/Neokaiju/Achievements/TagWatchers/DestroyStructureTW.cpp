// Fill out your copyright notice in the Description page of Project Settings.


#include "DestroyStructureTW.h"
#include "Neokaiju/Structures/BreakableStructures/BreakableStructureSubsystem.h"
#include "Neokaiju/Structures/BreakableStructures/BreakableStructure.h"
#include "Engine/World.h"

void UDestroyStructureTW::Init(UWorld* WorldValue)
{
	UBaseTagWatcher::Init(WorldValue);

	UBreakableStructureSubsystem* BreakableStructureSubsystem = WorldValue->GetSubsystem<UBreakableStructureSubsystem>();
	BreakableStructureSubsystem->OnStructureDestroyed.AddUniqueDynamic(this, &UDestroyStructureTW::StructureDestroyed);
}

void UDestroyStructureTW::GetSubsystemDependencies(TSet<TSubclassOf<USubsystem>>& OutDependencies) const
{
	OutDependencies.Add(UBreakableStructureSubsystem::StaticClass());
}


void UDestroyStructureTW::StructureDestroyed(ABreakableStructure* DestroyedStructure)
{
	if (IsValid(DestroyedStructure))
	{
		for (FName Tag : DestroyedStructure->Tags)
		{
			this->TagSeen(Tag);
		}
	}
}
