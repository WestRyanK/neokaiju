// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ThrowArrow.generated.h"

/**
 * 
 */
UCLASS()
class NEOKAIJU_API UThrowArrow : public UUserWidget
{
	GENERATED_BODY()
	
public:
private:
	void UpdateDirection(FVector2D blockWorldPosition, FVector direction);
};
