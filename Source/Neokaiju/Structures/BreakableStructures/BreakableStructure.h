// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "Neokaiju/Structures/BreakableStructures/BreakableStructureAttributeSet.h"
#include "Neokaiju/Structures/InteractiveStructure.h"
#include "Delegates/Delegate.h"
#include "Neokaiju/Core/SaveGame/SaveableInterface.h"
#include "BreakableStructure.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStructureUnstable, ABreakableStructure*, UnstableStructure);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStructureDestroyed, ABreakableStructure*, DestroyedStructure);

UCLASS()
class NEOKAIJU_API ABreakableStructure : public AActor, public IInteractiveStructure, public IAbilitySystemInterface, public ISaveableInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABreakableStructure();



	// Variables
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BreakableStructure")
		bool UseAutoScaleToMesh = true;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BreakableStructure")
		float LinearDampingAmount;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BreakableStructure")
		float AngularDampingAmount;

	// I need a property that tells me this because reading the value of Mesh->IsSimulatingPhysics() doesn't return the right value during EndPlay event. -RW
	UPROPERTY(BlueprintReadWrite, BlueprintGetter = GetIsSimulatingPhysics, BlueprintSetter = SetIsSimulatingPhysics)
		bool bIsSimulatingPhysics;

	UFUNCTION(BlueprintGetter)
		bool GetIsSimulatingPhysics() const;

	UFUNCTION(BlueprintSetter)
		void SetIsSimulatingPhysics(bool bIsSimulatingPhysicsValue);


	// Components

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Component")
		class USceneComponent* ScaleComponent;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Component")
		class UStaticMeshComponent* Mesh;

	UPROPERTY(BlueprintReadWrite, Category = "Component")
		class UBoxComponent* RightBox;

	UPROPERTY(BlueprintReadWrite, Category = "Component")
		class UBoxComponent* LeftBox;

	UPROPERTY(BlueprintReadWrite, Category = "Component")
		class UBoxComponent* TopBox;

	UPROPERTY(BlueprintReadWrite, Category = "Component")
		class UBoxComponent* BottomBox;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		class USceneComponent* AttachPosition;

	//Events
	UPROPERTY(BlueprintAssignable, Category = "Event")
		FOnStructureUnstable OnStructureUnstable;

	UPROPERTY(BlueprintAssignable, Category = "Event")
		FOnStructureDestroyed OnStructureDestroyed;


	UPROPERTY(BlueprintReadOnly)
		class UAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY()
		class UBreakableStructureAttributeSet* AttributeSet;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Default", Meta = (ClampMin = "0", ClampMax = "5", DisplayPriority = "1"))
		int BlockTier;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "BreakableStructure")
		class TSubclassOf<class UGameplayEffect> ImpulseDamageStructureEffect;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "BreakableStructure")
		TArray<TSubclassOf<class UGameplayEffect>> InitialEffects;

	UFUNCTION(BlueprintCallable, Category = "BreakableStructure")
		void ResizeBounds();

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		TArray<TSubclassOf<class UGameplayEffect>> TierGEList;

	UPROPERTY(BlueprintReadOnly)
		TArray<ABreakableStructure*>ConstrainedNeighbors;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Physics constrain to another structure
	void InitConstraint(class UPhysicsConstraintComponent* Constraint, TSet<AActor*> ConstraintNeighbors);

	void SetInitialEffects();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void OnConstruction(const FTransform& Transform) override;

	// From Interactive Structure Interface
	void OnInteract();

	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	void ListenForAttributeChanges();

	void UpdateTier(int Tier);

	virtual void HealthChanged(const FOnAttributeChangeData& Data);

	UFUNCTION(BlueprintCallable, Category = "BreakableStructure")
		virtual void UpdateCharacterStats();

	UFUNCTION(BlueprintCallable, Category = "BreakableStructure")
		virtual void DestroyStructure();

	UFUNCTION(BlueprintCallable, Category = "BreakableStructure")
		virtual void OnMeshHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);


	// Blueprint Events
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void PlayOnDamagedFX(float OldHealth, float NewHealth);
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void PlayOnDestroyedFX();

	// ISaveableInterface Methods
	virtual void BeginLoadState();

	virtual void EndLoadState();

protected:
	UPROPERTY(BlueprintReadOnly)
		bool bIsLoadingState;

private:
	bool bIsBreakableStructureBeingDestroyed = false;

	void UpdateOverlaps();
};

