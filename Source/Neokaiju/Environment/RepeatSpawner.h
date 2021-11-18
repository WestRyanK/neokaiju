// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RepeatSpawner.generated.h"


UENUM(BlueprintType)
enum class ESpawnTrigger : uint8
{
	ON_DESTROYED	UMETA(DisplayName = "On Destroyed"),
	ON_MOVED		UMETA(DisplayName = "On Moved")
};

UCLASS()
class NEOKAIJU_API ARepeatSpawner : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		class USceneComponent* SpawnInitialPosition;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		class USceneComponent* SpawnAdjustedPosition;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		class UBoxComponent* SpawnArea;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		class UStaticMeshComponent* Mesh;

	//UPROPERTY(BlueprintReadOnly, EditAnywhere)
	//	float SpawnVelocity;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		float AdjustPositionDelay;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		float DelayAfterSpawnedActorMoved;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		ESpawnTrigger SpawnTrigger = ESpawnTrigger::ON_DESTROYED;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TSubclassOf<AActor> ActorToSpawn;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TSubclassOf<class UShake> SpawnShakeClass;

	// Sets default values for this actor's properties
	ARepeatSpawner();

	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
		void Spawn();

	UFUNCTION()
		void AdjustPosition();

	void PlaySpawnShake();

	UPROPERTY(BlueprintReadWrite)
		AActor* SpawnedActor;

	void TryStartSpawn();

	bool bIsWaitingToSpawn = false;

	void ApplyTagsToSpawned();

};
