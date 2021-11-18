// Fill out your copyright notice in the Description page of Project Settings.


#include "TestGrabbable.h"

// Sets default values
ATestGrabbable::ATestGrabbable()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATestGrabbable::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ATestGrabbable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATestGrabbable::Grab_Implementation(USceneComponent* Parent, FName SocketName, FVector GrabLocation, bool GrabGroup)
{
	//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::White, TEXT("Grab Called on Object"));
}

bool ATestGrabbable::CanBeGrabbed_Implementation(AKaijuCharacter* Kaiju)
{
	return true;
}

void ATestGrabbable::Throw_Implementation(FVector Direction, float LaunchVelocity, TSubclassOf<UGameplayEffect> ThrownObjectEffect)
{
	//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::White, TEXT("Throw Called on Object"));
}

