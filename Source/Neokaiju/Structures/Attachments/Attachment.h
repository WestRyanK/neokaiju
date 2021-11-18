// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "Neokaiju/Structures/InteractiveStructure.h"
#include "Neokaiju/Structures/BreakableStructures/BreakableStructure.h"
#include "Neokaiju/Structures/Attachments/AttachmentAttributeSet.h"
#include "Attachment.generated.h"

UCLASS()
class NEOKAIJU_API AAttachment : public AActor, public IInteractiveStructure, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAttachment();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Component")
		class USceneComponent* ScaleComponent;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Component")
		class UStaticMeshComponent* Mesh;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Component")
		class UBoxComponent* AttachBox;

	UPROPERTY(BlueprintReadOnly)
		class UAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY()
		class UAttachmentAttributeSet* AttributeSet;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GE")
		class TSubclassOf<class UGameplayEffect> InitialAttributeValues;

	UPROPERTY(BlueprintReadOnly, Category = "BuildingBlock")
		class ABreakableStructure* BlockAttached;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// From Interactive Structure Interface
	void OnInteract();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void DestroyAttachment();

	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UFUNCTION(BlueprintCallable, Category = "EventHandler")
		virtual void AttachedBlockUnstable(ABreakableStructure* UnstableStructure);
	UFUNCTION(BlueprintCallable, Category = "EventHandler")
		virtual void AttachedBlockDestroyed(AActor* DestroyedActor);

	// Blueprint Events
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void PlayOnDamagedFX();
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void PlayOnDestroyedFX();

private:

	void InitAttachBlock();

	void SetAttachedIfBreakableStructure(AActor* Actor);

};
