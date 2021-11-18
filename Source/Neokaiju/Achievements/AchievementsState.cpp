// Fill out your copyright notice in the Description page of Project Settings.


#include "AchievementsState.h"
#include "Neokaiju/Achievements/BaseAchievement.h"
#include "Engine/DataTable.h"

UAchievementsState::UAchievementsState()
{
}

void UAchievementsState::PostInitProperties()
{
	Super::PostInitProperties();
	this->InitAchievements();
	this->SelectHighlightedAchievements();
}

void UAchievementsState::GetEarnedAchievements(TArray<UBaseAchievement*>& EarnedAchievements) const
{
	EarnedAchievements.Empty();
	for (UBaseAchievement* Achievement : this->AchievementsList)
	{
		if (Achievement->GetIsAchievementEarned())
		{
			EarnedAchievements.Add(Achievement);
		}
	}
}

void UAchievementsState::InitAchievements()
{
	if (IsValid(this->EarnableAchievementsTable))
	{
		TArray<FName> RowNames = this->EarnableAchievementsTable->GetRowNames();
		this->EarnableAchievements.Empty();
		for (FName RowName : RowNames)
		{
			FAchievementDefinition* AchievementDefinition = this->EarnableAchievementsTable->FindRow<FAchievementDefinition>(RowName, TEXT(""));
			if (AchievementDefinition->bIsEnabled)
			{
				AchievementDefinition->UniqueName = RowName.ToString();
				UBaseAchievement* Achievement = NewObject<UBaseAchievement>(this, AchievementDefinition->AchievementClass);
				Achievement->Definition = *AchievementDefinition;

				this->EarnableAchievements.Add(AchievementDefinition->UniqueName, Achievement);
				this->AchievementsList.Add(Achievement);
			}
		}
	}
}

void UAchievementsState::SelectHighlightedAchievements()
{
	this->HighlightedAchievements.Empty();

	TArray<UBaseAchievement*> ShuffledAchievements = TArray<UBaseAchievement*>(this->AchievementsList);
	// Taken from UKismetArrayLibrary::GenericArray_Shuffle()
	int32 LastIndex = ShuffledAchievements.Num() - 1;
	for (int32 i = 0; i <= LastIndex; ++i)
	{
		int32 Index = FMath::RandRange(i, LastIndex);
		if (i != Index)
		{
			ShuffledAchievements.Swap(i, Index);
		}
	}

	int32 CountToAdd = this->HighlightedAchievementCount;
	if (ShuffledAchievements.Num() < CountToAdd)
	{
		CountToAdd = ShuffledAchievements.Num();
	}

	for (int32 i = 0; i < CountToAdd; i++)
	{
		this->HighlightedAchievements.Add(ShuffledAchievements[i]);
	}
}

TArray<UBaseAchievement*> UAchievementsState::GetHighlightedAchievements() const
{
	return this->HighlightedAchievements;
}

void UAchievementsState::LoadProgresses(TMap<FString, int32> AchievementProgresses)
{
	for (UBaseAchievement* Achievement : this->AchievementsList)
	{
		FString AchievementName = Achievement->Definition.UniqueName;
		int32* AchievementProgress = AchievementProgresses.Find(AchievementName);
		if (AchievementProgress)
		{
			Achievement->LoadProgress(*AchievementProgress);
		}
	}
}
