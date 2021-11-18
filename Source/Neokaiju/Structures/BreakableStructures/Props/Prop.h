// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Neokaiju/Structures/BreakableStructures/BreakableStructure.h"
#include "Prop.generated.h"

class ABuildingBlock;

UCLASS()
class NEOKAIJU_API AProp : public ABreakableStructure
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProp();

	virtual void BeginDestroy() override;

	// Variables
	UPROPERTY(BlueprintReadWrite, Category = "Prop")
	TArray<ABreakableStructure*> AttachedBlocks;

	UPROPERTY(BlueprintReadWrite, Category = "Prop")
	bool HasAttachedBlocks;


	//Components

	UPROPERTY(BlueprintReadWrite, Category = "Component")
		class UPhysicsConstraintComponent* RightConstraint;

	UPROPERTY(BlueprintReadWrite, Category = "Component")
		class UPhysicsConstraintComponent* LeftConstraint;

	UPROPERTY(BlueprintReadWrite, Category = "Component")
		class UPhysicsConstraintComponent* TopConstraint;

	UPROPERTY(BlueprintReadWrite, Category = "Component")
		class UPhysicsConstraintComponent* BottomConstraint;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "EventHandler")
	void AttachedBlockUnstable(ABreakableStructure* UnstableStructure);
	UFUNCTION(BlueprintCallable, Category = "EventHandler")
	void AttachedBlockDestroyed(AActor* DestroyedActor);

	UFUNCTION(BlueprintCallable, Category = "Prop")
	bool GetHasAttachedBlocks();

	UFUNCTION()
		void OnHit(
			UPrimitiveComponent* HitComp,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComponent,
			FVector NormalImpulse,
			const FHitResult& Hit);

	UFUNCTION()
		void OnOverlap(
			UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult& SweepResult);

private:

	void InitAttachedBlocks();

	void AddToAttachedBlocksIfBuildingBlock(AActor* Actor);

	bool ReleaseConstraint(ABreakableStructure* AttachedBlock);
};
