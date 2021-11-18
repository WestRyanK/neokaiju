// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "KaijuCharacterController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSkipIntroRequested);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPauseUnpauseRequested);
/**
 * 
 */
UCLASS()
class NEOKAIJU_API AKaijuCharacterController : public APlayerController
{
	GENERATED_BODY()
	
public:
	virtual void SetupInputComponent() override;

	UPROPERTY(BlueprintAssignable)
		FOnSkipIntroRequested OnSkipIntroRequested;

	UPROPERTY(BlueprintAssignable)
		FOnPauseUnpauseRequested OnPauseUnpauseRequested;

	UFUNCTION()
		void RequestSkipIntro();

	UFUNCTION()
		void RequestPauseUnpause();
};
