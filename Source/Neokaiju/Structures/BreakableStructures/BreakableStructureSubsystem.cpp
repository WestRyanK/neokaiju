// Fill out your copyright notice in the Description page of Project Settings.


#include "BreakableStructureSubsystem.h"
#include "Subsystems/SubsystemCollection.h"
#include "Neokaiju/Structures/BreakableStructures/BuildingBlocks/BuildingBlock.h"
#include "Neokaiju/Structures/BreakableStructures/BuildingBlocks/BlockManager.h"
#include "Neokaiju/Structures/BreakableStructures/RubbleManager.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"


void UBreakableStructureSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	this->BlockManager = NewObject<UBlockManager>(this);
	this->RubbleManager = NewObject<URubbleManager>(this);
}

void UBreakableStructureSubsystem::StructureDestroyed(ABreakableStructure* DestroyedStructure)
{
	this->OnStructureDestroyed.Broadcast(DestroyedStructure);
}

void UBreakableStructureSubsystem::AddRubble(AActor* Rubble)
{
	//this->RubbleManager->AddRubble(Rubble);
}
