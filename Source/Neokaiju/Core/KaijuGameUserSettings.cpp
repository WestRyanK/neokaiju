// Fill out your copyright notice in the Description page of Project Settings.


#include "KaijuGameUserSettings.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"
#include "FMODBlueprintStatics.h"

float UKaijuGameUserSettings::GetMusicVolume() const
{
	return this->MusicVolume;
}

float UKaijuGameUserSettings::GetSoundVolume() const
{
	return this->SoundVolume;
}

bool UKaijuGameUserSettings::GetRumbleEnabled() const
{
	return this->RumbleEnabled;
}

int32 UKaijuGameUserSettings::GetGraphicsQuality() const
{
	return this->GraphicsQuality;
}

void UKaijuGameUserSettings::SetMusicVolume(float NewValue)
{
	UFMODBlueprintStatics::VCASetVolume(this->MusicVCA, NewValue);
	this->MusicVolume = NewValue;
}

void UKaijuGameUserSettings::SetSoundVolume(float NewValue)
{
	UFMODBlueprintStatics::VCASetVolume(this->SoundEffectVCA, NewValue);
	this->SoundVolume = NewValue;
}

void UKaijuGameUserSettings::SetRumbleEnabled(bool NewValue)
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this->World.Get(), 0);
	if (IsValid(PlayerController))
	{
		PlayerController->bForceFeedbackEnabled = NewValue;
	}
	this->RumbleEnabled = NewValue;
}

void UKaijuGameUserSettings::SetGraphicsQuality(int32 NewValue)
{
	if (NewValue == 0) {
		this->SetResolutionScaleValueEx(this->LowQualityScale);
	}
	else if (NewValue == 1) {
		this->SetResolutionScaleValueEx(this->MidQualityScale);
	}
	else if (NewValue == 2) {
		this->SetResolutionScaleValueEx(this->HighQualityScale);
	}
	this->ApplySettings(false);

	this->GraphicsQuality = NewValue;
}

UKaijuGameUserSettings* UKaijuGameUserSettings::LoadNeokaijuSettings(UWorld* World, UFMODVCA* MusicVCAValue, UFMODVCA* SoundEffectVCAValue)
{
	UKaijuGameUserSettings* GameUserSettings = Cast<UKaijuGameUserSettings>(GEngine->GameUserSettings);
	GameUserSettings->World = World;
	GameUserSettings->MusicVCA = MusicVCAValue;
	GameUserSettings->SoundEffectVCA = SoundEffectVCAValue;

	if (IsValid(GameUserSettings))
	{
		// Apply settings values to VCAs etc...
		GameUserSettings->SetMusicVolume(GameUserSettings->GetMusicVolume());
		GameUserSettings->SetSoundVolume(GameUserSettings->GetSoundVolume());
		GameUserSettings->SetRumbleEnabled(GameUserSettings->GetRumbleEnabled());
		GameUserSettings->SetGraphicsQuality(GameUserSettings->GetGraphicsQuality());
	}

	return GameUserSettings;
}
