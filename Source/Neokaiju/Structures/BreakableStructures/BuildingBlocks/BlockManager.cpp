// Fill out your copyright notice in the Description page of Project Settings.


#include "BlockManager.h"
#include "BuildingBlock.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/Classes/Kismet/GameplayStatics.h"
#include "Neokaiju/Structures/BreakableStructures/BreakableStructure.h"
#include "BuildingBlock.h"
#include "Engine/World.h"


// Check which blocks need physics enabled
void UBlockManager::CheckBlocks(TArray<ABuildingBlock*> Neighbors)
{
	for (ABuildingBlock* Block : Neighbors)
	{
		if (IsValid(Block))
		{
			if (!Block->GetIsSimulatingPhysics())
			{
				TArray<ABuildingBlock*> FallingGroup = TArray<ABuildingBlock*>();
				FallingGroup = FindFallingGroup(FallingGroup, Block);

				if (FallingGroup.Num() > 0)
				{
					for (ABuildingBlock* FallingBlock : FallingGroup)
					{
						FallingBlock->SetPhysics();
					}
				}
			}
		}
	}
}

bool UBlockManager::CanEnablePhysics(TArray<ABuildingBlock*> Neighbors)
{
	bool PhysicsSet = false;
	for (ABuildingBlock* Block : Neighbors)
	{
		if (IsValid(Block))
		{
			if (!Block->GetIsSimulatingPhysics())
			{
				TArray<ABuildingBlock*> FallingGroup = TArray<ABuildingBlock*>();
				FallingGroup = FindFallingGroup(FallingGroup, Block);

				if (FallingGroup.Num() > 0 && FallingGroup.Num() < (MaxGrabGroupSize - 1))
				{
					// true if the falling group is valid and not the max size
					PhysicsSet = true;
					break;

				}
			}
			else
			{
				// return true if the physics are already set
				PhysicsSet = true;
				break;
			}
		}
	}
	// false if the structure can't have physics enabled, true if it can, or already has
	return PhysicsSet;
}

bool UBlockManager::BlocksAttached(ABuildingBlock* Source, ABuildingBlock* Destination)
{
	bool Attached = false;
	for (ABuildingBlock* Block : Source->Neighbors) 
	{
		if (!Attached) {
			TArray<ABuildingBlock*> FallingGroup = TArray<ABuildingBlock*>();
			FallingGroup = FindFallingGroup(FallingGroup, Block);
			if (FallingGroup.Num() > 0)
			{
				for (ABuildingBlock* ConnectedBlock : FallingGroup)
				{
					if (ConnectedBlock == Destination) {
						Attached = true;
					}
				}
			}
		}
	}
	
	return Attached;
}

float UBlockManager::MinimumLevelToGrabGroup(TArray<ABuildingBlock*> Neighbors)
{
	float MinimumLevel = 0.0f;
	for (ABuildingBlock* Block : Neighbors)
	{
		if (IsValid(Block))
		{
			
			TArray<ABuildingBlock*> FallingGroup = TArray<ABuildingBlock*>();
			FallingGroup = FindFallingGroup(FallingGroup, Block);

			if (FallingGroup.Num() > 0 && FallingGroup.Num() < (MaxGrabGroupSize - 1))
			{
				for (ABuildingBlock* FallingBlock : FallingGroup)
				{
					float CurrentBlock = FallingBlock->GetAbilitySystemComponent()->GetNumericAttribute(UBreakableStructureAttributeSet::GetMinimumKaijuLevelToGrabAttribute());
					if (MinimumLevel < CurrentBlock)
					{
						// For each block in the falling group, find the highest minimum level for grabbing
						
						MinimumLevel = CurrentBlock;
					}
				}

			}
			else
			{
				return 5.0f;
			}
			
		}
	}
	
	// return the highest minimum level to grab in the falling group
	return MinimumLevel;
}

TArray<ABuildingBlock*> UBlockManager::FindFallingGroup(TArray<ABuildingBlock*> &FallingGroup, ABuildingBlock* NextBlock)
{
	if (IsValid(NextBlock))
	{
		FallingGroup.AddUnique(NextBlock);

		if ((FallingGroup.Num() >= MaxGroupSize) || (NextBlock->GetBlockType() == BLOCK_FOUNDATION))
		{
			//UE_LOG(LogTemp, Display, TEXT("Max falling group size or foundation reached on block chunk"));
			for (ABuildingBlock* Block : FallingGroup)
			{
				//UE_LOG(LogTemp, Display, TEXT("%s"), *Block->GetName());
			}
			return TArray<ABuildingBlock*>();
		}

		for (ABreakableStructure* Structure : NextBlock->Neighbors)
		{
			ABuildingBlock* Block = Cast<ABuildingBlock>(Structure);

			if (IsValid(Block))
			{
				if (!FallingGroup.Contains(Block))
				{
					if (FindFallingGroup(FallingGroup, Block).Num() == 0)
					{
						// Somewhere in the chunk is a foundation block, or chunk size is too large to enable physics
						return TArray<ABuildingBlock*>();
					}
				}
			}
		}
	}

	return FallingGroup;
}

