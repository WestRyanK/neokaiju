// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Grabbable.h"
#include "TestGrabbable.generated.h"

class UGameplayEffect;

UCLASS()
class NEOKAIJU_API ATestGrabbable : public AActor, public IGrabbable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATestGrabbable();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Grab")
	void Grab(USceneComponent* Parent, FName SocketName, FVector GrabLocation, bool GrabGroup);
	virtual void Grab_Implementation(USceneComponent* Parent, FName SocketName, FVector GrabLocation, bool GrabGroup);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Grab")
	bool CanBeGrabbed(AKaijuCharacter* Kaiju);
	virtual bool CanBeGrabbed_Implementation(AKaijuCharacter* Kaiju);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Grab")
	void Throw(FVector Direction, float LaunchVelocity, TSubclassOf<UGameplayEffect> ThrownObjectEffect);
	virtual void Throw_Implementation(FVector Direction, float LaunchVelocity, TSubclassOf<UGameplayEffect> ThrownObjectEffect);

};
