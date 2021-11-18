// Fill out your copyright notice in the Description page of Project Settings.


#include "TagSeenAchievement.h"
#include "Neokaiju/Achievements/AchievementSubsystem.h"

UTagSeenAchievement::UTagSeenAchievement()
{
	if (IsValid(this->GetWorld()) && IsValid(this->AchievementSubsystem))
	{
		this->AchievementSubsystem->OnTagSeen.AddUniqueDynamic(this, &UTagSeenAchievement::TagSeen);
	}
}

void UTagSeenAchievement::TagSeen(FName SeenTag)
{
	if (SeenTag == this->Definition.AchievementTimerStartTag)
	{
		this->StartAchievement();
	}

	if (SeenTag == this->Definition.TagName)
	{
		this->AddToProgress(1);
	}
}
