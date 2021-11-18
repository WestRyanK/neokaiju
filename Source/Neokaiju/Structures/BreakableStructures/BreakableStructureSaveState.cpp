// Fill out your copyright notice in the Description page of Project Settings.


#include "BreakableStructureSaveState.h"
#include "Neokaiju/Core/SaveGame/NeokaijuSaveGame.h"
#include "Neokaiju/Structures/BreakableStructures/BreakableStructure.h"

void UBreakableStructureSaveStateManager::SaveStateInLevel(UNeokaijuSaveGame* SaveGame, FString LevelName, TArray<TScriptInterface<ISaveableInterface>>& SaveablesInLevel)
{
	if (!SaveGame->SavedLevels.Contains(LevelName))
	{
		SaveGame->SavedLevels.Add(LevelName, FNeokaijuSaveGameLevel());
	}

	FNeokaijuSaveGameLevel* LevelSaveState = SaveGame->SavedLevels.Find(LevelName);
	if (LevelSaveState)
	{
		LevelSaveState->BreakableStructures.Empty();
		for (TScriptInterface<ISaveableInterface> Saveable : SaveablesInLevel)
		{
			ABreakableStructure* Structure = Cast<ABreakableStructure>(Saveable.GetObject());
			if (IsValid(Structure))
			{
				FBreakableStructureSaveState SaveState;
				SaveState.StructureHealth = Structure->AttributeSet->GetHealth();
				SaveState.bIsSimulatingPhysics = Structure->GetIsSimulatingPhysics();
				SaveState.StructureLocation = Structure->GetActorLocation();
				SaveState.StructureRotation = Structure->GetActorRotation();

				LevelSaveState->BreakableStructures.Add(Structure->GetSaveKey(), SaveState);
			}
		}
	}
}


void UBreakableStructureSaveStateManager::LoadStateInLevel(UNeokaijuSaveGame* SaveGame, FString LevelName, TArray<TScriptInterface<ISaveableInterface>>& SaveablesInLevel)
{
	FNeokaijuSaveGameLevel* LevelSaveState = SaveGame->SavedLevels.Find(LevelName);
	if (LevelSaveState)
	{
		for (TScriptInterface<ISaveableInterface> Saveable : SaveablesInLevel)
		{
			ABreakableStructure* Structure = Cast<ABreakableStructure>(Saveable.GetObject());
			if (IsValid(Structure))
			{
				FBreakableStructureSaveState* SaveState = LevelSaveState->BreakableStructures.Find(Structure->GetSaveKey());
				if (SaveState)
				{
					Structure->BeginLoadState();
					Structure->SetIsSimulatingPhysics(SaveState->bIsSimulatingPhysics);
					if (SaveState->bIsSimulatingPhysics)
					{
						Structure->SetActorLocation(SaveState->StructureLocation, false, nullptr, ETeleportType::TeleportPhysics);
						Structure->SetActorRotation(SaveState->StructureRotation, ETeleportType::TeleportPhysics);
					}
					Structure->AttributeSet->SetHealth(SaveState->StructureHealth);
					Structure->EndLoadState();
				}
				else
				{
					Structure->BeginLoadState();
					Structure->Destroy();
				}
			}
		}
	}
}
