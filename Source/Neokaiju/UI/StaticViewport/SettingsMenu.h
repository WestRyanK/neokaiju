// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include "Neokaiju/UI/InGameMenus/BackButtonEnabledMenu.h"
#include "Neokaiju/UI/NavigatableMenu.h"
#include "SettingsMenu.generated.h"

class UKaijuGameUserSettings;
/**
 *
 */

UCLASS()
class NEOKAIJU_API USettingsMenu : public UNavigatableMenu
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	UFUNCTION(BlueprintCallable)
		void SaveMusicVolume(float NewValue);
	UFUNCTION(BlueprintCallable)
		void SaveSoundVolume(float NewValue);
	UFUNCTION(BlueprintCallable)
		void SaveIsRumbleEnabled(bool NewValue);
	UFUNCTION(BlueprintCallable)
		void SaveGraphicsQuality(int32 NewValue);
protected:
	UPROPERTY(BlueprintReadOnly)
		UKaijuGameUserSettings* GameUserSettings;
};
