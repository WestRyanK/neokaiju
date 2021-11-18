// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Swarm.generated.h"

class AKaijuCharacter;

UCLASS()
class NEOKAIJU_API ASwarm : public AActor
{
	GENERATED_BODY()
public:	
	// Sets default values for this actor's properties
	ASwarm();

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Characteristics")
		float windupSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Characteristics")
		float speedMultiplier;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Characteristics")
		float cooldownTime;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Characteristics")
		float startDelay;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Lightning")
		float lightningSize;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Lightning")
		float lightningTime;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack")
		float knockbackAmount;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Characteristics")
		int32 maxSwarmAmount;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Characteristics")
		float scatterTime;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Characteristics")
		int32 numDirs;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Characteristics")
		float patroling;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Characteristics")
		float AlertRadius;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile")
		uint8 dashesToEscape;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile")
		float maxShake;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile")
		float projectileVelocity;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile")
		float projectileLifetime;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Characteristics")
		float targetingRange;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Characteristics")
		float stopDistance;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	AActor* targetActor;
	float speed;
	float windup;
	bool atTarget;
	bool stopped;
	bool coolingdown;
	FVector enemyScale;
	int32 swarmAmount;
	FVector boundingBox;
	float SwarmRadius;
	bool scattered;
	bool characterSeen;
	float patrolTime;
	FVector lastDir;
	bool characterGrabbed;
	uint8 numDashes;
	FVector grabLocation;
	float initGrab;
	bool lockedon;
	bool solid;
	AActor* projectile;
};
