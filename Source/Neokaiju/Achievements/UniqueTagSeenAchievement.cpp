// Fill out your copyright notice in the Description page of Project Settings.


#include "UniqueTagSeenAchievement.h"
#include "Neokaiju/Achievements/AchievementSubsystem.h"

UUniqueTagSeenAchievement::UUniqueTagSeenAchievement()
{
	if (IsValid(this->GetWorld()) && IsValid(this->AchievementSubsystem))
	{
		this->AchievementSubsystem->OnTagSeen.AddUniqueDynamic(this, &UUniqueTagSeenAchievement::TagSeen);
	}
}

void UUniqueTagSeenAchievement::TagSeen(FName SeenTag)
{

	if (this->IsTagPrefix(SeenTag, this->Definition.AchievementTimerStartTag))
	{
		this->StartAchievement();
	}

	if (this->IsTagPrefix(SeenTag, this->Definition.TagName) && !this->SeenTags.Contains(SeenTag))
	{
		this->SeenTags.Add(SeenTag);
		this->AddToProgress(1);
	}
}

bool UUniqueTagSeenAchievement::IsTagPrefix(FName InTag, FName PrefixTag)
{
	FString InTagString = InTag.ToString();
	FString PrefixTagString = PrefixTag.ToString() + PREFIX_SEPARATOR;
	int32 Index = InTagString.Find(PrefixTagString);
	bool bIsPrefix = (Index == 0);
	return bIsPrefix;
}
