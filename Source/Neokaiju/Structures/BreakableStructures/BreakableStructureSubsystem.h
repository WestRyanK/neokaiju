// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Neokaiju/Structures/BreakableStructures/BreakableStructure.h"
#include "BreakableStructureSubsystem.generated.h"

class ABuildingBlock;
class AActor;
class UBlockManager;
class URubbleManager;


/**
 *
 */
UCLASS()
class NEOKAIJU_API UBreakableStructureSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection);

	UPROPERTY(BlueprintReadOnly)
		UBlockManager* BlockManager;

	UPROPERTY(BlueprintReadOnly)
		URubbleManager* RubbleManager;

	UPROPERTY(BlueprintAssignable)
		FOnStructureDestroyed OnStructureDestroyed;

	UFUNCTION(BlueprintCallable)
		void StructureDestroyed(ABreakableStructure* DestroyedStructure);

	UFUNCTION(BlueprintCallable)
		void AddRubble(AActor* Rubble);
};
