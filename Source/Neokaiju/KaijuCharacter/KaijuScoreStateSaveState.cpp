// Fill out your copyright notice in the Description page of Project Settings.


#include "KaijuScoreStateSaveState.h"
#include "Neokaiju/Core/SaveGame/NeokaijuSaveGame.h"
#include "Neokaiju/KaijuCharacter/KaijuScoreState.h"

void UKaijuScoreStateSaveStateManager::SaveStateInLevel(UNeokaijuSaveGame* SaveGame, FString LevelName, TArray<TScriptInterface<ISaveableInterface>>& SaveablesInLevel)
{
	if (!SaveGame->SavedLevels.Contains(LevelName))
	{
		SaveGame->SavedLevels.Add(LevelName, FNeokaijuSaveGameLevel());
	}

	FNeokaijuSaveGameLevel* LevelSaveState = SaveGame->SavedLevels.Find(LevelName);
	if (LevelSaveState)
	{
		LevelSaveState->KaijuScoreStates.Empty();
		for (TScriptInterface<ISaveableInterface> Saveable : SaveablesInLevel)
		{
			AKaijuScoreState* KaijuScoreState = Cast<AKaijuScoreState>(Saveable.GetObject());
			if (IsValid(KaijuScoreState))
			{
				FKaijuScoreStateSaveState SaveState = FKaijuScoreStateSaveState();
				SaveState.BabyKaijusSaved = KaijuScoreState->GetBabyKaijusSaved();
				SaveState.PropertyDamageDealt = KaijuScoreState->GetPropertyDamageDealt();
				SaveState.TimeBonusMultiplier = KaijuScoreState->GetTimeBonusMultiplier();
				SaveState.BonusRemainingTime = KaijuScoreState->GetBonusRemainingTime();
				SaveState.BlocksDestroyedForBonus = KaijuScoreState->GetBlocksDestroyedForBonus();

				LevelSaveState->KaijuScoreStates.Add(KaijuScoreState->GetSaveKey(), SaveState);
			}
		}
	}
}


void UKaijuScoreStateSaveStateManager::LoadStateInLevel(UNeokaijuSaveGame* SaveGame, FString LevelName, TArray<TScriptInterface<ISaveableInterface>>& SaveablesInLevel)
{
	FNeokaijuSaveGameLevel* LevelSaveState = SaveGame->SavedLevels.Find(LevelName);
	if (LevelSaveState)
	{
		for (TScriptInterface<ISaveableInterface> Saveable : SaveablesInLevel)
		{
			AKaijuScoreState* KaijuScoreState = Cast<AKaijuScoreState>(Saveable.GetObject());
			if (IsValid(KaijuScoreState))
			{
				FKaijuScoreStateSaveState* SaveState = LevelSaveState->KaijuScoreStates.Find(KaijuScoreState->GetSaveKey());
				if (SaveState)
				{
					KaijuScoreState->BeginLoadState();
					KaijuScoreState->SetBabyKaijusSaved(SaveState->BabyKaijusSaved);
					KaijuScoreState->SetPropertyDamageDealt(SaveState->PropertyDamageDealt);
					KaijuScoreState->SetTimeBonusMultiplier(SaveState->TimeBonusMultiplier);
					KaijuScoreState->SetBonusRemainingTime(SaveState->BonusRemainingTime);
					KaijuScoreState->SetBlocksDestroyedForBonus(SaveState->BlocksDestroyedForBonus);
					KaijuScoreState->EndLoadState();
				}
			}
		}
	}
}

