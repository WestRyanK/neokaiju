// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NeoKaiju/Ability/Core/Abilities/BaseGameplayAbility.h"
#include "GrabAbility.generated.h"

class AKaijuCharacter;
class ABuildingBlock;
class USplineComponent;
class USplineMeshComponent;

/**
 *
 */
UCLASS()
class NEOKAIJU_API UGrabAbility : public UBaseGameplayAbility
{
	GENERATED_BODY()

public:
	UGrabAbility();

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability | Grab")
		float GrabRange = 100.0f;

	// The widths to try grabbing objects with. Start with narrower
	// widths assuming that the player is targeting blocks directly in
	// the aim direction, and increase the width for broader searches
	// in case they missed.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability | Grab")
		TArray<float> GrabWidths;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability | Grab")
		TSubclassOf<UGameplayEffect> OwnerEffectWhileGrabbing;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability | Grab")
		float ThrowVelocityMultiplier = 1000.0f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability | Grab")
		FName MouthSocketName;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability | Grab")
		FName FeetCenterSocketName;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability | Grab")
		float ThrowArcThickness = 10.0f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability | Grab")
		float ThrowArcFrequency = 10.0f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability | Grab")
		float ThrowArcSimTime = 2.0f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability | Grab")
		float GrabDelay = 0.0f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability | Grab")
		FColor ThrowArcColor = FColor::Cyan;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability | Grab")
		bool bIsReleaseInputToThrow = true;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability | Grab")
		float RotateAimDirectionUpByDegrees = 10.0f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability | Grab")
		float TimeBetweenGrabbedExistsChecks = 0.2f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability | Grab")
		TSubclassOf<UGameplayEffect> ThrownObjectEffect;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability | Grab")
		USplineComponent* SplineRef;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability | Grab")
		FVector GrabLocation = FVector(0, 0, 0);

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability | Grab")
		FVector AimDirection = FVector(0, 0, 0);

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;

	virtual bool CommitAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo);

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled);

protected:

	AActor* GrabbedActorRef;
	AActor* TraceForward();
	TSet<AActor*> TraceDown();

	UFUNCTION()
		void CheckGrabbedExists(int32 CheckNumber);

	bool IsKaijuStandingOnConnectedBlock(ABuildingBlock* GrabbedBlock);

	bool IsKaijuStandingOnGrabbedBlock(ABuildingBlock* GrabbedBlock);

	bool CanKaijuGrab(AActor* GrabbedActor);

	bool CanKaijuGrabBlockGroup(ABuildingBlock* GrabbedBlock);

	FVector GetAdjustedThrowAim();

	void StartAbilityTasks();

	void PredictProjectilePath();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void OnThrow();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void OnGrab();

	UPROPERTY(BlueprintReadOnly)
		AKaijuCharacter* Character;

	UFUNCTION()
		void Tick(float DeltaTime);

	UFUNCTION()
		void ReachComplete(float TimeRemaining);

	UFUNCTION()
		void ReachCancel(float TimeRemaining);

private:
	FActiveGameplayEffectHandle OwnerEffectWhileGrabbingHandle;

	UFUNCTION()
		void ThrowObject(float TimeHeld);

	UFUNCTION()
		void DropObject();
};
