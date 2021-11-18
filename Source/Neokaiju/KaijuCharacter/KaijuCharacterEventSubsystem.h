// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include <Neokaiju\Structures\Attachments\Claw.h>
#include "KaijuCharacterEventSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnKaijuDied);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnKaijuScoreChanged, float, NewScore);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnKaijuHealthChanged, float, OldHealth, float, NewHealth, float, MaxHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnKaijuLaserChargeChanged, float, OldCharge, float, NewCharge, float, MaxCharge);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnKaijuTrappedByClaw);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnKaijuReleasedFromClaw);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnKaijuHealthRegenerationChanged, float, NewHealthRegeneration, float, MaxHealthRegeneration);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBabyKaijuSaved);

/**
 * 
 */
UCLASS()
class NEOKAIJU_API UKaijuCharacterEventSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:

	UPROPERTY(BlueprintAssignable)
		FOnKaijuDied OnKaijuDied;

	UPROPERTY(BlueprintAssignable)
		FOnBabyKaijuSaved OnBabyKaijuSaved;

	UPROPERTY(BlueprintAssignable)
		FOnKaijuScoreChanged OnKaijuScoreChanged;

	UPROPERTY(BlueprintAssignable)
		FOnKaijuHealthChanged OnKaijuHealthChanged;

	UPROPERTY(BlueprintAssignable)
		FOnKaijuLaserChargeChanged OnKaijuLaserChargeChanged;

	UPROPERTY(BlueprintAssignable)
		FOnKaijuTrappedByClaw OnKaijuTrappedByClaw;

	UPROPERTY(BlueprintAssignable)
		FOnKaijuReleasedFromClaw OnKaijuReleasedFromClaw;

	//UPROPERTY(BlueprintAssignable)
	//	FOnKaijuHealthRegenerationChanged OnKaijuHealthRegenerationChanged;

	UFUNCTION(BlueprintCallable)
		void KaijuDied();

	UFUNCTION(BlueprintCallable)
		void BabyKaijuSaved();

	UFUNCTION(BlueprintCallable)
		void KaijuHealthChanged(float OldHealth, float NewHealth, float MaxHealth);

	UFUNCTION(BlueprintCallable)
		void KaijuLaserChargeChanged(float OldCharge, float NewCharge, float MaxCharge);

	UFUNCTION(BlueprintCallable)
		void KaijuTrappedByClaw();

	UFUNCTION(BlueprintCallable)
		void KaijuReleasedFromClaw();

	//UFUNCTION(BlueprintCallable)
	//	void KaijuHealthRegenerationChanged(float NewHealthRegeneration, float MaxHealthRegeneration);
};
