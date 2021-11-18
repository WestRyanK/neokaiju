// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BlockManager.generated.h"

class ABuildingBlock;

UCLASS()
class NEOKAIJU_API UBlockManager : public UObject
{
	GENERATED_BODY()

public:

	// Chunks with more blocks than this will not simulate physics
	UPROPERTY(EditAnywhere, Category = "BlockManager")
	int MaxGroupSize = 50;

	// Chunks with more blocks than this will not simulate physics when grabbed
	UPROPERTY(EditAnywhere, Category = "BlockManager")
	int MaxGrabGroupSize = 10;

	void CheckBlocks(TArray<ABuildingBlock*> Neighbors);

	bool CanEnablePhysics(TArray<ABuildingBlock*> Neighbors);

	bool BlocksAttached(ABuildingBlock* Source, ABuildingBlock* Destination);

	float MinimumLevelToGrabGroup(TArray<ABuildingBlock*> Neighbors);

private:

	// Continually adds neighbors to group until we can determine if we are attached 
	//to a foundation block or there are too many in the chunk
	TArray<ABuildingBlock*> FindFallingGroup(TArray<ABuildingBlock*> &Group, ABuildingBlock* Block);
};
