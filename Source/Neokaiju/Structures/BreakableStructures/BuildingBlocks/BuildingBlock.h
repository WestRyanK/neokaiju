// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Neokaiju/Structures/BreakableStructures/BreakableStructure.h"
#include "Neokaiju/Structures/Grabbable.h"
#include "BuildingBlock.generated.h"

UENUM(BlueprintType)
enum BuildingBlockType {
	BLOCK_STICKY,
	BLOCK_BRITTLE,
	BLOCK_FOUNDATION,
	BLOCK_INDEPENDENT
};

class UGameplayEffect;
class UTextRenderComponent;

UCLASS()
class NEOKAIJU_API ABuildingBlock : public ABreakableStructure, public IGrabbable
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABuildingBlock();


	// Variables

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default", Meta = (DisplayAfter="BlockTier"))
		TEnumAsByte<BuildingBlockType> BlockType;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default", Meta = (DisplayAfter="BlockType"))
		bool AttachRight = true;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default", Meta = (DisplayAfter="AttachRight"))
		bool AttachBottom = true;

	// Neighbors contains other building blocks and props attached to this block
	UPROPERTY(BlueprintReadOnly, Category = "BuildingBlock")
		TArray<ABuildingBlock*> Neighbors;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "BreakableStructure")
		float MinHitImpulseToDamage = 100000.0f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "BreakableStructure")
		float HitImpulseDamageMultiplier = 1 / 100000.0f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "BreakableStructure")
		float ThrowDamageTierMultiplier = 1.0f;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
		bool bAngularBreakable = false;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
		bool bLinearBreakable = false;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
		float AngularLimit = 100000.0f;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
		float LinearLimit = 100000.0f;

	// Components

	UPROPERTY(BlueprintReadWrite, Category = "Component")
		class UPhysicsConstraintComponent* RightConstraint;
	UPROPERTY(BlueprintReadWrite, Category = "Component")
		class UPhysicsConstraintComponent* LeftConstraint;
	UPROPERTY(BlueprintReadWrite, Category = "Component")
		class UPhysicsConstraintComponent* BottomConstraint;
	UPROPERTY(BlueprintReadWrite, Category = "Component")
		class UPhysicsConstraintComponent* TopConstraint;
	
	UPROPERTY(BlueprintReadOnly)
		class UMaterialInstanceDynamic* BaseMat;

	UPROPERTY(BlueprintReadOnly)
		class UMaterialInstanceDynamic* WindowMat;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default", Meta = (DisplayAfter = "AttachBottom"))
		class UMaterialInstance* BaseMatSource;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default", Meta = (DisplayAfter = "BaseMatSource"))
		class UMaterialInstance* WindowMatSource;

	// This does not do what we want it to do...
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default", Meta = (DisplayAfter = "WindowMatSource"))
		class USkeletalMesh* DestructibleMesh;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		class UTextRenderComponent* TierIndicator;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		class UTextRenderComponent* TypeIndicator;

	UPROPERTY()
		TSet<AActor*> RightNeighbors;
	UPROPERTY()
		TSet<AActor*> LeftNeighbors;
	UPROPERTY()
		TSet<AActor*> TopNeighbors;
	UPROPERTY()
		TSet<AActor*> BottomNeighbors;

	UPROPERTY()
		FGameplayTagContainer InteractableTags;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void UpdateIndicators();


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void HealthChanged(const FOnAttributeChangeData& Data) override;

	UFUNCTION(BlueprintCallable, Category = "BuildingBlock")
		TEnumAsByte<BuildingBlockType> GetBlockType();

	//void SetBlockManager(ABlockManager* Manager);

	UFUNCTION(BlueprintCallable, Category = "BuildingBlock")
		void SetPhysics();

	void OnMeshHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;

	virtual void DestroyStructure() override;

	void Grab_Implementation(USceneComponent* Parent, FName SocketName, FVector GrabLocation, bool GrabGroup);

	bool CanBeGrabbed_Implementation(AKaijuCharacter* Kaiju);

	void Throw_Implementation(FVector Direction, float LaunchVelocity, TSubclassOf<UGameplayEffect> ThrownObjectEffect);

	void UpdateDeform(AActor* Neighbor);

	void UpdateCracks();

private:

	void InitNeighbors();

	void AddToNeighborsIfBuildingBlock(AActor* Actor);

	bool bIsBuildingBlockBeingDestroyed = false;
	//UFUNCTION()
	//	void ThrownDelay();
};
