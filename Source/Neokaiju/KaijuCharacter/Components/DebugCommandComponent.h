// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AbilitySystemComponent.h"
#include "DebugCommandComponent.generated.h"

class UUserWidget;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class NEOKAIJU_API UDebugCommandComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UDebugCommandComponent();

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TSubclassOf<UGameplayEffect> LevelDownEffect;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TSubclassOf<UGameplayEffect> LevelUpEffect;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TSubclassOf<UGameplayEffect> TopUpEffect;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TSubclassOf<UGameplayEffect> InstantKillEffect;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TSubclassOf<UGameplayEffect> InstantDamageEffect;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TSubclassOf<UUserWidget> AchievementDebugWidgetType;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UFUNCTION()
		void Slomo1_0();
	UFUNCTION()
		void Slomo0_5();
	UFUNCTION()
		void Slomo0_2();
	UFUNCTION()
		void Slomo0_1();
	UFUNCTION()
		void ShowAbilitySystem();
	UFUNCTION()
		void HideAbilitySystem();
	UFUNCTION()
		void PreviousAbilityTarget();
	UFUNCTION()
		void NextAbilityTarget();
	UFUNCTION()
		void ShowAchievements();
	UFUNCTION()
		void HideAchievements();
	UFUNCTION()
		void ToggleAchievements();
	UFUNCTION()
		void LevelDown();
	UFUNCTION()
		void LevelUp();
	UFUNCTION()
		void InstantDamage();
	UFUNCTION()
		void InstantKill();
	UFUNCTION()
		void InstantWin();
	UFUNCTION()
		void TopUp();
	UFUNCTION()
		void SaveBabyKaiju();
	UFUNCTION()
		void DestroyWeapon();
	UFUNCTION()
		void DebugHelp();

	UPROPERTY()
		UUserWidget* SpawnedAchievementDebugWidget;

	UFUNCTION()
		void ExecCommand(FString Command);

	UFUNCTION()
		void DisplayMessage(FString Message, float Duration = 0.5f);

	bool bIsAbilitySystemShown = false;

};
