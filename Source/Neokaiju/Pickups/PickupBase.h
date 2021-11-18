// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Neokaiju/Core/SaveGame/SaveableInterface.h"
#include "PickupBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPickupPickedUp, APickupBase*, PickedUpPickup);

class AKaijuCharacter;
class UStaticMeshComponent;
class UGameplayEffect;

UCLASS()
class NEOKAIJU_API APickupBase : public AActor, public ISaveableInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APickupBase();

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
		UStaticMeshComponent* Mesh;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TSubclassOf<UGameplayEffect> EffectToApply;

	UFUNCTION()
		void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
			const FHitResult& SweepResult);

	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION(BlueprintCallable)
		virtual void PickUp(AKaijuCharacter* Character);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly)
		bool bIsLoadingState;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void OnPickedUp(AKaijuCharacter* Character);

	bool bWasPickedUp = false;
};
