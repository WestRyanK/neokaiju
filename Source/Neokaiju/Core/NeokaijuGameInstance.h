// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "NeokaijuGameInstance.generated.h"

class UKaijuGameUserSettings;
class UFMODVCA;
/**
 *
 */
UCLASS(Blueprintable)
class NEOKAIJU_API UNeokaijuGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
		UKaijuGameUserSettings* GameSettings;

	UFUNCTION(BlueprintCallable)
		void LoadGameSettings();

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		UFMODVCA* MusicVCA;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		UFMODVCA* SoundEffectsVCA;
};
