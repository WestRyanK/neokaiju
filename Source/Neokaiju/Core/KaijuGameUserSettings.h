// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "KaijuGameUserSettings.generated.h"

class UFMODVCA;
class UWorld;
/**
 *
 */
UCLASS(config = GameUserSettings, configdonotcheckdefaults, Blueprintable)
class NEOKAIJU_API UKaijuGameUserSettings : public UGameUserSettings
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = Settings)
		float GetMusicVolume() const;
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = Settings)
		float GetSoundVolume() const;
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = Settings)
		bool GetRumbleEnabled() const;
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = Settings)
		int32 GetGraphicsQuality() const;

	UFUNCTION(BlueprintCallable, Category = Settings)
		void SetMusicVolume(float NewValue);
	UFUNCTION(BlueprintCallable, Category = Settings)
		void SetSoundVolume(float NewValue);
	UFUNCTION(BlueprintCallable, Category = Settings)
		void SetRumbleEnabled(bool NewValue);
	UFUNCTION(BlueprintCallable, Category = Settings)
		void SetGraphicsQuality(int32 NewValue);

	UFUNCTION(BlueprintCallable)
		static UKaijuGameUserSettings* LoadNeokaijuSettings(UWorld* World, UFMODVCA* MusicVCAValue, UFMODVCA* SoundEffectVCAValue);

protected:
	UPROPERTY()
		UFMODVCA* MusicVCA;

	UPROPERTY()
		UFMODVCA* SoundEffectVCA;

	TWeakObjectPtr<UWorld> World;

	UPROPERTY(config)
		float MusicVolume = 1.0f;

	UPROPERTY(config)
		float SoundVolume = 1.0f;

	UPROPERTY(config)
		bool RumbleEnabled = true;

	UPROPERTY(config)
		int32 GraphicsQuality = 2;

private:
	float LowQualityScale = 40.0f;
	float MidQualityScale = 75.0f;
	float HighQualityScale = 100.0f;
};
