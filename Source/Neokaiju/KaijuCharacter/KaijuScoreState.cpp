// Fill out your copyright notice in the Description page of Project Settings.


#include "KaijuScoreState.h"
#include "Neokaiju/Core/SaveGame/NeoKaijuSaveGameSubsystem.h"
#include "Neokaiju/KaijuCharacter/KaijuCharacterEventSubsystem.h"
#include "Neokaiju/Structures/BreakableStructures/BreakableStructureSubsystem.h"
#include "Neokaiju/Structures/BreakableStructures/BreakableStructureAttributeSet.h"
#include "Neokaiju/Achievements/AchievementsState.h"
#include "Neokaiju/Achievements/BaseAchievement.h"
#include "Neokaiju/Core/PlayLevelGameMode.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "Engine/World.h"
#include "TimerManager.h"

void AKaijuScoreState::BeginPlay()
{
	Super::BeginPlay();
	UBreakableStructureSubsystem* BreakableStructureSubsystem = this->GetWorld()->GetSubsystem<UBreakableStructureSubsystem>();
	BreakableStructureSubsystem->OnStructureDestroyed.AddUniqueDynamic(this, &AKaijuScoreState::OnStructureDestroyed);

	UKaijuCharacterEventSubsystem* KaijuCharacterEventSubsystem = this->GetWorld()->GetSubsystem<UKaijuCharacterEventSubsystem>();
	KaijuCharacterEventSubsystem->OnBabyKaijuSaved.AddUniqueDynamic(this, &AKaijuScoreState::AddSavedBabyKaiju);


	if (IsValid(this->AchievementsStateClass))
	{
		this->AchievementsState = NewObject<UAchievementsState>(this, this->AchievementsStateClass);
	}

	if (IsValid(this->ObjectivesStateClass))
	{
		this->ObjectivesState = NewObject<UObjectivesState>(this, this->ObjectivesStateClass);
	}

	GetWorld()->GetTimerManager().SetTimer(TimerHandle_RefreshCooldown, this, &AKaijuScoreState::BonusCooldown, cooldownRefreshRate, true);
}

void AKaijuScoreState::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle_RefreshCooldown);
	this->UpdateHighScore();

	Super::EndPlay(EndPlayReason);
}

void AKaijuScoreState::UpdateHighScore()
{
	UWorld* World = this->GetWorld();
	if (IsValid(World))
	{
		UNeoKaijuSaveGameSubsystem* SaveGameSubsystem = this->GetWorld()->GetSubsystem<UNeoKaijuSaveGameSubsystem>();
		if (IsValid(SaveGameSubsystem))
		{
			SaveGameSubsystem->UpdateHighScore(this);
		}
	}
}

void AKaijuScoreState::TotalScoreChanged()
{
	this->OnTotalScoreChanged.Broadcast(this->GetTotalScore());
}

float AKaijuScoreState::GetTotalScore() const
{
	float TotalScore = this->GetTotalScoreWithoutAchievementPoints();

	TArray<UBaseAchievement*> EarnedAchievements;
	this->AchievementsState->GetEarnedAchievements(EarnedAchievements);
	for (UBaseAchievement* EarnedAchievement : EarnedAchievements)
	{
		TotalScore += EarnedAchievement->Definition.CompletionRewardValue;
	}

	return TotalScore;
}

float AKaijuScoreState::GetTotalScoreWithoutAchievementPoints() const
{
	// Currently, the TotalScoreWithoutAchievementPoints is just the PropertyDamageDealt.
	// As the game is being played, streak multipliers are accounted for in this value.
	return this->PropertyDamageDealt;
}

TArray<float> AKaijuScoreState::GetMaxBonusCooldownTimes() const
{
	return this->maxBonusCooldownTimes;
}
float AKaijuScoreState::GetMaxBonusCooldownTime() const
{
	int multiplierIndex = TimeBonusMultiplier - minTimeBonusMultiplier;
	if (multiplierIndex >= 1 && multiplierIndex < maxBonusCooldownTimes.Num()) {
		return maxBonusCooldownTimes[multiplierIndex];
	}
	else
	{
		return 10.0f;
	}
}

void AKaijuScoreState::OnStructureDestroyed(ABreakableStructure* DestroyedStructure)
{
	APlayLevelGameMode* GameMode = Cast<APlayLevelGameMode>(this->GetWorld()->GetAuthGameMode());
	if (IsValid(GameMode) && GameMode->CanScoreBeChanged())
	{
		IAbilitySystemInterface* ASCActor = Cast<IAbilitySystemInterface>(DestroyedStructure);
		if (ASCActor)
		{

			UAbilitySystemComponent* ASC = ASCActor->GetAbilitySystemComponent();
			float PropertyValue = ASC->GetNumericAttribute(UBreakableStructureAttributeSet::GetStructureValueAttribute());
			this->SetPropertyDamageDealt(this->GetPropertyDamageDealt() + PropertyValue * TimeBonusMultiplier);

			ABuildingBlock* buildingBlock = Cast<ABuildingBlock>(DestroyedStructure);
			if (buildingBlock) {
				BonusStreakRefresh();
			}
		}
	}
}

void AKaijuScoreState::SetPropertyDamageDealt(float PropertyDamageDealtValue)
{
	if (this->PropertyDamageDealt != PropertyDamageDealtValue)
	{
		this->PropertyDamageDealt = PropertyDamageDealtValue;
		this->OnPropertyDamageDealtChanged.Broadcast(PropertyDamageDealtValue);
		this->TotalScoreChanged();
	}
}

float AKaijuScoreState::GetPropertyDamageDealt() const
{
	return this->PropertyDamageDealt;
}

void AKaijuScoreState::SetBabyKaijusSaved(float BabyKaijusSavedValue)
{
	if (this->BabyKaijusSaved != BabyKaijusSavedValue)
	{
		this->BabyKaijusSaved = BabyKaijusSavedValue;
		this->OnBabyKaijusSavedChanged.Broadcast(BabyKaijusSavedValue);
	}
}

float AKaijuScoreState::GetBabyKaijusSaved() const
{
	return this->BabyKaijusSaved;
}

float AKaijuScoreState::GetTimeBonusMultiplier()
{
	return this->TimeBonusMultiplier;
}

void AKaijuScoreState::SetTimeBonusMultiplier(float TimeBonusMultiplierValue)
{
	if (this->TimeBonusMultiplier != TimeBonusMultiplierValue)
	{
		this->TimeBonusMultiplier = TimeBonusMultiplierValue;
		this->OnTimeBonusMultiplierChanged.Broadcast(TimeBonusMultiplierValue);
	}
}

float AKaijuScoreState::GetBonusRemainingTime()
{
	return this->BonusRemainingTime;
}

void AKaijuScoreState::SetBonusRemainingTime(float BonusRemainingTimeValue)
{
	if (this->BonusRemainingTime != BonusRemainingTimeValue)
	{
		this->BonusRemainingTime = BonusRemainingTimeValue;
		this->OnBonusRemainingTimeChanged.Broadcast(BonusRemainingTimeValue);
	}
}

float AKaijuScoreState::GetBlocksDestroyedForBonus()
{
	return this->BlocksDestroyedForBonus;
}

void AKaijuScoreState::SetBlocksDestroyedForBonus(float BlocksDestroyedForBonusValue)
{
	if (this->BlocksDestroyedForBonus != BlocksDestroyedForBonusValue)
	{
		this->BlocksDestroyedForBonus = BlocksDestroyedForBonusValue;
		this->OnBlocksDestroyedForBonusChanged.Broadcast(BlocksDestroyedForBonusValue);
	}
}

float AKaijuScoreState::GetLongestStreak()
{
	return LongestStreak;
}

void AKaijuScoreState::SetLongestStreak(float LongestStreakValue)
{
	if (this->LongestStreak != LongestStreakValue)
	{
		this->LongestStreak = LongestStreakValue;
		this->OnLongestStreakChanged.Broadcast(LongestStreakValue);
	}
}

float AKaijuScoreState::GetCurrentStreak()
{
	return CurrentStreak;
}

void AKaijuScoreState::SetCurrentStreak(float CurrentStreakValue)
{
	if (this->CurrentStreak != CurrentStreakValue)
	{
		this->CurrentStreak = CurrentStreakValue;
		this->OnCurrentStreakChanged.Broadcast(CurrentStreakValue);
	}
}

void AKaijuScoreState::BonusCooldown()
{
	if (BonusRemainingTime > 0.0f && TimeBonusMultiplier > minTimeBonusMultiplier)
	{
		SetBonusRemainingTime(BonusRemainingTime - cooldownRefreshRate);
		SetCurrentStreak(CurrentStreak + cooldownRefreshRate);
		if (CurrentStreak > LongestStreak)
		{
			//UE_LOG(LogTemp, Display, TEXT("Streak %f"), CurrentStreak);
			SetLongestStreak(CurrentStreak);
		}
	}
	else
	{
		if (TimeBonusMultiplier > minTimeBonusMultiplier)
		{
			SetTimeBonusMultiplier(minTimeBonusMultiplier);
			SetCurrentStreak(0.0f);
		}
		SetBonusRemainingTime(0);
	}
}

void AKaijuScoreState::BonusStreakRefresh()
{
	SetBonusRemainingTime(GetMaxBonusCooldownTime());
	SetBlocksDestroyedForBonus(BlocksDestroyedForBonus + 1);
	if (BlocksDestroyedForBonus == maxBlocksDestroyedForBonus)
	{
		if (TimeBonusMultiplier != maxTimeBonusMultiplier)
			SetTimeBonusMultiplier(TimeBonusMultiplier + 1);
		SetBlocksDestroyedForBonus(0);
	}
}

void AKaijuScoreState::AddSavedBabyKaiju()
{
	this->SetBabyKaijusSaved(this->GetBabyKaijusSaved() + 1);
}
