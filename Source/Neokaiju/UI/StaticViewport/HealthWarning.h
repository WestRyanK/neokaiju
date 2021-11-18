// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Neokaiju/KaijuCharacter/KaijuCharacterEventSubsystem.h"
#include "HealthWarning.generated.h"

/**
 * 
 */
UCLASS()
class NEOKAIJU_API UHealthWarning : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	UPROPERTY(EditAnywhere)
		float warningThreshold;
private:
	UKaijuCharacterEventSubsystem* KaijuCharacterEventSubsystem;
	void SetWarningActive(bool active);
	UFUNCTION()
		void CheckHealthChange(float OldHealth, float NewHealth, float MaxHealth);
protected:
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
		bool warningIsActive;
	UFUNCTION(BlueprintImplementableEvent)
		void DeactivateWarning();
};
