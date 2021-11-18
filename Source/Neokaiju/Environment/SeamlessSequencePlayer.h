// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TimerManager.h"
#include "Camera/PlayerCameraManager.h"
#include "SeamlessSequencePlayer.generated.h"

class ALevelSequenceActor;
class AActor;
class ACameraActor;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSeamlessSequencePlayerFinished);
/**
 * A helper class that plays a level sequence with a specific tag. The player optionally allows for easing in and out of the camera movement for the sequence.
 * In order to support easing out, the level sequence must contain EaseOutTime number of seconds at the end of the sequence with nothing happening.
 * If you don't include that empty part in the sequence, it will start easing during the rest of the animation.
 * We need it to start easing during the animation because as soon as the sequence is over, the camera snaps back to its original position instead of the position it was in
 * at the end of the sequence.
 */
UCLASS(Blueprintable)
class NEOKAIJU_API USeamlessSequencePlayer : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"))
		static USeamlessSequencePlayer* CreateSeamlessSequencePlayerFromTags(
			UObject* WorldContextObject,
			FName SequenceTag,
			FName CameraTag,
			float EaseInTimeValue = 0.0f,
			float EaseOutTimeValue = 0.0f,
			bool bIsInputDisabledDuringSequenceValue = true,
			EViewTargetBlendFunction EaseInBlendFunctionValue = EViewTargetBlendFunction::VTBlend_Linear,
			EViewTargetBlendFunction EaseOutBlendFunctionValue = EViewTargetBlendFunction::VTBlend_Linear,
			float EaseInBlendExponentValue = 0.0f,
			float EaseOutBlendExponentValue = 0.0f);

	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"))
		static USeamlessSequencePlayer* CreateSeamlessSequencePlayerFromRefs(
			UObject* WorldContextObject,
			ALevelSequenceActor* SequenceValue,
			ACameraActor* SequenceCameraValue,
			float EaseInTimeValue = 0.0f,
			float EaseOutTimeValue = 0.0f,
			bool bIsInputDisabledDuringSequenceValue = true,
			EViewTargetBlendFunction EaseInBlendFunctionValue = EViewTargetBlendFunction::VTBlend_Linear,
			EViewTargetBlendFunction EaseOutBlendFunctionValue = EViewTargetBlendFunction::VTBlend_Linear,
			float EaseInBlendExponentValue = 0.0f,
			float EaseOutBlendExponentValue = 0.0f);

	UPROPERTY(BlueprintAssignable)
		FOnSeamlessSequencePlayerFinished OnSeamlessSequencePlayerFinished;

	UFUNCTION(BlueprintCallable)
		void StopSequence();

	UFUNCTION(BlueprintCallable)
		void PlaySequence();

private:
	USeamlessSequencePlayer();

	void InitFromTags(FName SequenceTag, FName CameraTag);

	void InitFromRefs(ALevelSequenceActor* SequenceValue, ACameraActor* SequenceCameraValue);

	void InitCommon(
		float EaseInTimeValue,
		float EaseOutTimeValue,
		bool bIsInputDisabledDuringSequenceValue,
		EViewTargetBlendFunction EaseInBlendFunctionValue,
		EViewTargetBlendFunction EaseOutBlendFunctionValue,
		float EaseInBlendExponentValue,
		float EaseOutBlendExponentValue);

	ALevelSequenceActor* Sequence;

	ACameraActor* SequenceCamera;

	AActor* OriginalViewTarget;

	float EaseInTime;

	float EaseOutTime;

	EViewTargetBlendFunction EaseInBlendFunction;

	EViewTargetBlendFunction EaseOutBlendFunction;

	float EaseInBlendExponent;

	float EaseOutBlendExponent;

	bool bIsInputDisabledDuringSequence;

	UFUNCTION()
		void OnSequenceFinished();

	UFUNCTION()
		void OnEaseInCompleted();

	UFUNCTION()
		void OnEaseOutCompleted();

	FTimerHandle EaseInTimerHandle;

	FTimerHandle EaseOutTimerHandle;

	FTimerHandle PlaySequenceTimerHandle;

	template<class T>
	T* GetActorWithTag(TSubclassOf<T> ClassType, FName Tag)
	{
		TArray<AActor*> ActorsWithTag;
		UGameplayStatics::GetAllActorsOfClassWithTag(this, ClassType, Tag, ActorsWithTag);
		if (ActorsWithTag.Num() == 0)
		{
			UE_LOG(LogTemp, Error, TEXT("No Actors with tag: %s"), *Tag.ToString());
			return nullptr;
		}
		if (ActorsWithTag.Num() > 1)
		{
			UE_LOG(LogTemp, Error, TEXT("More than one Actor with tag: %s"), *Tag.ToString());
			return nullptr;
		}

		T* Actor = Cast<T>(ActorsWithTag[0]);
		return Actor;
	}
};
