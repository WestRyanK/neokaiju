// Fill out your copyright notice in the Description page of Project Settings.


#include "SurvivalAchievement.h"
#include "Neokaiju/Achievements/AchievementSubsystem.h"
#include "Engine/GameEngine.h"
#include "Neokaiju/Core/PlayLevelGameMode.h"
#include "Engine/World.h"

USurvivalAchievement::USurvivalAchievement()
{
	if (IsValid(this->GetWorld()) && IsValid(this->AchievementSubsystem))
	{
		this->AchievementSubsystem->OnTagSeen.AddUniqueDynamic(this, &USurvivalAchievement::TagSeen);
	}
}


void USurvivalAchievement::TagSeen(FName SeenTag)
{
	if (SeenTag == this->Definition.TagName)
	{
		if (IsValid(this->GetWorld()))
		{
			this->GameMode = Cast<APlayLevelGameMode>(this->GetWorld()->GetAuthGameMode());
			if (IsValid(this->GameMode))
			{
				if (this->GameMode->CountdownTimer->GetTimeRemaining() <= this->Definition.TimeLimitSeconds)
				{
					this->AddToProgress(1);
				}
			}
		}
	}
}