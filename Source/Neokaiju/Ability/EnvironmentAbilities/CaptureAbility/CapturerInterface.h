// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CapturerInterface.generated.h"


// Any actor that wants to use the capture ability to capture another actor must implement this interface and override
// its OnCapturedActorStruggle() function. This way the capture ability can broadcast to this function 
// so that the capturing actor can perform any checks/animations/fx it needs to in response to the captured actor's input.

// This class does not need to be modified.
UINTERFACE(MinimalAPI, Blueprintable)
class UCapturerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 *
 */
class NEOKAIJU_API ICapturerInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void OnCapturedActorStruggle(int StruggleAttemptNum, int StrugglesRemainingToEscape, float TimeSinceCaptureStarted);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void OnCapturedActorEscape(int StruggleAttemptNum, float TimeSinceCaptureStarted);
};
