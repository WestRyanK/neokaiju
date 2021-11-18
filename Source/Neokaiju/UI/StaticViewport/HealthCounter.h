// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Neokaiju/KaijuCharacter/KaijuCharacterEventSubsystem.h"
#include <Neokaiju\UI\StaticViewport\IndividualHealthIndicator.h>
#include "HealthCounter.generated.h"

/**
 * 
 */
UCLASS()
class NEOKAIJU_API UHealthCounter : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
private:

	UKaijuCharacterEventSubsystem* KaijuCharacterEventSubsystem;
	UFUNCTION()
		void UpdateHealth(float OldHealth, float NewHealth, float MaxHealth);
	//UFUNCTION()
	//	void UpdateRegeneration(float NewHealthRegeneration, float MaxHealthRegeneration);
protected:
	UPROPERTY(BlueprintReadOnly)
		float currentHealth;
	UPROPERTY(BlueprintReadOnly)
		float maxHealth;
	UPROPERTY(BlueprintReadOnly)
		float currentHealthRegeneration;
	UPROPERTY(BlueprintReadOnly)
		float maxHealthRegeneration;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<UIndividualHealthIndicator*> healthIndicators;
	UFUNCTION(BlueprintNativeEvent)
		void RefreshUI();
	virtual void RefreshUI_Implementation();
};
