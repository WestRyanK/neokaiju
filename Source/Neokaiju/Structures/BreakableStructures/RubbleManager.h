// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Containers/Queue.h"
#include "UObject/NoExportTypes.h"
#include "RubbleManager.generated.h"

//class TQueue<>;

/**
 * 
 */
UCLASS()
class NEOKAIJU_API URubbleManager : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Default")
		int MaxMeshes = 10;

	UPROPERTY()
		int TotalMeshes = 0;
	
	TQueue<AActor*> ActiveMeshes = {};

	UFUNCTION(BlueprintCallable)
		void AddRubble(AActor* Rubble);

protected:
	void RemoveRubble();
};
