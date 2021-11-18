// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Neokaiju/KaijuCharacter/KaijuCharacterEventSubsystem.h"
#include "LaserJuiceBar.generated.h"

/**
 *
 */
UCLASS()
class NEOKAIJU_API ULaserJuiceBar : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
private:

	UKaijuCharacterEventSubsystem* KaijuCharacterEventSubsystem;

	UFUNCTION()
		void UpdateRemainingCharge(float OldChargeValue, float NewChargeValue, float MaxChargeValue);

protected:
	UPROPERTY(BlueprintReadOnly)
		float OldCharge;
	UPROPERTY(BlueprintReadOnly)
		float CurrentCharge;
	UPROPERTY(BlueprintReadOnly)
		float MaxCharge;
	UFUNCTION(BlueprintImplementableEvent)
		void RefreshUI();
};
