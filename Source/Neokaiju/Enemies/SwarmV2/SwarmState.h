// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SwarmState.generated.h"

class ASwarmV2;

/**
 * 
 */
UCLASS(Blueprintable)
class NEOKAIJU_API USwarmState : public UObject
{
	GENERATED_BODY()

public:
	USwarmState();

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		ASwarmV2* SwarmRef;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void Enter(ASwarmV2* NewSwarm);
	virtual void Enter_Implementation(ASwarmV2* NewSwarm);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void Update(float DeltaTime);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void Exit();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void OnOuterBeginOverlap(
			UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool FromSweep,
			FHitResult SweepResult);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void OnOuterEndOverlap(
			UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void OnInnerBeginOverlap(
			UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool FromSweep,
			FHitResult SweepResult);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void OnInnerEndOverlap(
			UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex);


	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void OnBeginHitByLaser();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void OnEndHitByLaser();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void OnBeginOverlapByLaser();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void OnEndOverlapByLaser();

};
