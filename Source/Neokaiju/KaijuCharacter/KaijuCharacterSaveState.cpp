// Fill out your copyright notice in the Description page of Project Settings.


#include "KaijuCharacterSaveState.h"
#include "Neokaiju/Core/SaveGame/NeokaijuSaveGame.h"
#include "Neokaiju/KaijuCharacter/KaijuCharacter.h"

void UKaijuCharacterSaveStateManager::SaveStateInLevel(UNeokaijuSaveGame* SaveGame, FString LevelName, TArray<TScriptInterface<ISaveableInterface>>& SaveablesInLevel)
{
	if (!SaveGame->SavedLevels.Contains(LevelName))
	{
		SaveGame->SavedLevels.Add(LevelName, FNeokaijuSaveGameLevel());
	}

	FNeokaijuSaveGameLevel* LevelSaveState = SaveGame->SavedLevels.Find(LevelName);
	if (LevelSaveState)
	{
		LevelSaveState->KaijuCharacters.Empty();
		for (TScriptInterface<ISaveableInterface> Saveable : SaveablesInLevel)
		{
			AKaijuCharacter* KaijuCharacter = Cast<AKaijuCharacter>(Saveable.GetObject());
			if (IsValid(KaijuCharacter))
			{
				FKaijuCharacterSaveState SaveState = FKaijuCharacterSaveState();
				SaveState.KaijuLocation = KaijuCharacter->GetActorLocation();
				SaveState.KaijuRotation = KaijuCharacter->CharacterRotationPoint->GetComponentRotation();
				SaveState.DashDamage = KaijuCharacter->AttributeSet->GetHealth();
				SaveState.DashDistance = KaijuCharacter->AttributeSet->GetDashDistance();
				SaveState.DashDuration = KaijuCharacter->AttributeSet->GetDashDuration();
				SaveState.Health = KaijuCharacter->AttributeSet->GetHealth();
				SaveState.JumpHeight = KaijuCharacter->AttributeSet->GetJumpHeight();
				SaveState.KaijuLevel = KaijuCharacter->AttributeSet->GetKaijuLevel();
				SaveState.KaijuSizeScale = KaijuCharacter->AttributeSet->GetKaijuSizeScale();
				SaveState.LaserCharge = KaijuCharacter->AttributeSet->GetLaserCharge();
				SaveState.LaserDamage = KaijuCharacter->AttributeSet->GetLaserDamage();
				SaveState.MaxHealth = KaijuCharacter->AttributeSet->GetMaxHealth();
				SaveState.MaxLaserCharge = KaijuCharacter->AttributeSet->GetMaxLaserCharge();
				SaveState.WalkSpeed = KaijuCharacter->AttributeSet->GetWalkSpeed();

				LevelSaveState->KaijuCharacters.Add(KaijuCharacter->GetSaveKey(), SaveState);
			}
		}
	}
}


void UKaijuCharacterSaveStateManager::LoadStateInLevel(UNeokaijuSaveGame* SaveGame, FString LevelName, TArray<TScriptInterface<ISaveableInterface>>& SaveablesInLevel)
{
	FNeokaijuSaveGameLevel* LevelSaveState = SaveGame->SavedLevels.Find(LevelName);
	if (LevelSaveState)
	{
		for (TScriptInterface<ISaveableInterface> Saveable : SaveablesInLevel)
		{
			AKaijuCharacter* KaijuCharacter = Cast<AKaijuCharacter>(Saveable.GetObject());
			if (IsValid(KaijuCharacter))
			{
				FKaijuCharacterSaveState* SaveState = LevelSaveState->KaijuCharacters.Find(KaijuCharacter->GetSaveKey());
				if (SaveState)
				{
					KaijuCharacter->BeginLoadState();
					KaijuCharacter->SetActorLocation(SaveState->KaijuLocation);
					KaijuCharacter->CharacterRotationPoint->SetWorldRotation(SaveState->KaijuRotation);
					KaijuCharacter->AttributeSet->bShouldClampAttributes = false;
					KaijuCharacter->AttributeSet->SetDashDamage(SaveState->DashDamage);
					KaijuCharacter->AttributeSet->SetDashDistance(SaveState->DashDistance);
					KaijuCharacter->AttributeSet->SetDashDuration(SaveState->DashDuration);
					KaijuCharacter->AttributeSet->SetHealth(SaveState->Health);
					KaijuCharacter->AttributeSet->SetJumpHeight(SaveState->JumpHeight);
					KaijuCharacter->AttributeSet->SetKaijuLevel(SaveState->KaijuLevel);
					KaijuCharacter->AttributeSet->SetKaijuSizeScale(SaveState->KaijuSizeScale);
					KaijuCharacter->AttributeSet->SetLaserCharge(SaveState->LaserCharge);
					KaijuCharacter->AttributeSet->SetLaserDamage(SaveState->LaserDamage);
					KaijuCharacter->AttributeSet->SetMaxHealth(SaveState->MaxHealth);
					KaijuCharacter->AttributeSet->SetMaxLaserCharge(SaveState->MaxLaserCharge);
					KaijuCharacter->AttributeSet->SetWalkSpeed(SaveState->WalkSpeed);
					KaijuCharacter->AttributeSet->bShouldClampAttributes = true;
					KaijuCharacter->bShouldApplyInitialEffects = false;
					KaijuCharacter->EndLoadState();
				}
			}
		}
	}
}
