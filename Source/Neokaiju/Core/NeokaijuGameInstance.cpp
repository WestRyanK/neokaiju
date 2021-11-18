// Fill out your copyright notice in the Description page of Project Settings.


#include "NeokaijuGameInstance.h"
#include "Neokaiju/Core/KaijuGameUserSettings.h"

void UNeokaijuGameInstance::LoadGameSettings()
{
	this->GameSettings = UKaijuGameUserSettings::LoadNeokaijuSettings(this->GetWorld(), this->MusicVCA, this->SoundEffectsVCA);
}
