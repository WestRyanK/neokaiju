// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Neokaiju/Achievements/BaseAchievement.h"
#include "UniqueTagSeenAchievement.generated.h"

/**
 * 
 */
UCLASS()
class NEOKAIJU_API UUniqueTagSeenAchievement : public UBaseAchievement
{
	GENERATED_BODY()
	
public:
	UUniqueTagSeenAchievement();

protected:
	UFUNCTION()
		void TagSeen(FName SeenTag);

	bool IsTagPrefix(FName InTag, FName PrefixTag);

	const FString PREFIX_SEPARATOR = TEXT(".");

	TSet<FName> SeenTags;
};
