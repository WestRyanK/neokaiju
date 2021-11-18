// Fill out your copyright notice in the Description page of Project Settings.


#include "SettingsMenu.h"
#include "Neokaiju/Core/KaijuGameUserSettings.h"
#include "Neokaiju/Core/NeokaijuGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "FMODBlueprintStatics.h"

void USettingsMenu::NativeConstruct() 
{
	UNeokaijuGameInstance* GameInstance = this->GetWorld()->GetGameInstance<UNeokaijuGameInstance>();
	GameUserSettings = GameInstance->GameSettings;

	Super::NativeConstruct();
}

void USettingsMenu::SaveMusicVolume(float NewValue)
{
	GameUserSettings->SetMusicVolume(NewValue);
}
void USettingsMenu::SaveSoundVolume(float NewValue)
{
	GameUserSettings->SetSoundVolume(NewValue);
}

void USettingsMenu::SaveIsRumbleEnabled(bool NewValue)
{
	GameUserSettings->SetRumbleEnabled(NewValue);
}

void USettingsMenu::SaveGraphicsQuality(int32 NewValue)
{
	GameUserSettings->SetGraphicsQuality(NewValue);
}
