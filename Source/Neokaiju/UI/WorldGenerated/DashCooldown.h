// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DashCooldown.generated.h"

/**
 * 
 */
UCLASS()
class NEOKAIJU_API UDashCooldown : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
private:
	bool cooldownIsActive;
	void SetCooldownActive(bool active);
};
