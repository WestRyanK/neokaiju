// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Neokaiju/Structures/BreakableStructures/BuildingBlocks/BuildingBlock.h"
#include "LaserOnlyBlock.generated.h"

/**
 * 
 */
UCLASS()
class NEOKAIJU_API ALaserOnlyBlock : public ABuildingBlock
{
	GENERATED_BODY()

public:
	ALaserOnlyBlock();

protected:

	virtual void BeginPlay() override;
};
