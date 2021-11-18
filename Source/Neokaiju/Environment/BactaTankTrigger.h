// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Neokaiju/Environment/WaitForInputTrigger.h"
#include "BactaTankTrigger.generated.h"

class UShakerComponent;
class ABreakableStructure;
/**
 *
 */
UCLASS()
class NEOKAIJU_API ABactaTankTrigger : public AWaitForInputTrigger
{
	GENERATED_BODY()

public:
	virtual void OnBeginTrigger(AActor* TriggeringActor);

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TSubclassOf<class UShake> FloatingShakeClass;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		ABreakableStructure* BactaTankGlass;

protected:
	UShakerComponent* FloatingShaker;

	virtual void WaitForInputPressed();
};
