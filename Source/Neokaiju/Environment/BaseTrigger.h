// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "BaseTrigger.generated.h"

class AActor;
class UPrimitiveComponent;
/**
 *
 */
UCLASS()
class NEOKAIJU_API ABaseTrigger : public ATriggerBox
{
	GENERATED_BODY()

public:
	ABaseTrigger();

	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Trigger")
		TSubclassOf<AActor> TriggerForActorOfType;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Trigger")
		bool bCanBeTriggeredMultipleTimes = true;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Trigger")
		bool bCanInterruptTransitions = false;

	virtual void Tick(float DeltaTime) override;

protected:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void OnBeginTrigger_BP(AActor* TriggeringActor);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void OnEndTrigger_BP(AActor* TriggeringActor);

	UFUNCTION()
		void InitialOverlapCheck();

	UFUNCTION()
		virtual void OnBeginTrigger(AActor* TriggeringActor) { };

	UFUNCTION()
		virtual void OnEndTrigger(AActor* TriggeringActor) { };

	UFUNCTION(BlueprintCallable)
		void FinishTransition();

	UPROPERTY(BlueprintReadWrite, BlueprintGetter = GetIsTriggerEnabled, BlueprintSetter = SetIsTriggerEnabled)
		bool bIsTriggerEnabled = true;

	UFUNCTION(BlueprintGetter)
		bool GetIsTriggerEnabled() const;

	UFUNCTION(BlueprintSetter)
		void SetIsTriggerEnabled(bool bIsTriggerEnabledValue);

	UFUNCTION(BlueprintPure)
		bool GetIsTriggerActorInside() const;

	UFUNCTION(BlueprintPure)
		AActor* GetTriggerActor() const;

private:

	AActor* TriggerActor;

	bool bHasBeenTriggered = false;

	bool bIsInsideTrigger = false;

	bool bIsTransitioning = false;

	bool bIsEnterTriggerPending = false;

	bool bIsExitTriggerPending = false;

	bool ShouldPendingTriggerExecute(bool bIsTriggerPending);

	bool IsCorrectActorAndComponent(AActor* Actor, UPrimitiveComponent* Component);

	//bool ShouldSetTriggered(AActor* Actor);

	UFUNCTION()
		virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		virtual void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);



};
