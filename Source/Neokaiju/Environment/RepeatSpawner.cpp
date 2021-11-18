// Fill out your copyright notice in the Description page of Project Settings.


#include "RepeatSpawner.h"
#include "Components/ArrowComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Neokaiju/KaijuCharacter/Components/ShakerComponent.h"
#include "Engine/World.h"
#include "TimerManager.h"

// Sets default values
ARepeatSpawner::ARepeatSpawner()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	this->Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	this->RootComponent = this->Mesh;

	this->SpawnAdjustedPosition = CreateDefaultSubobject<USceneComponent>(TEXT("SpawnAdjustedPosition"));
	this->SpawnAdjustedPosition->SetupAttachment(this->RootComponent);

	this->SpawnInitialPosition = CreateDefaultSubobject<USceneComponent>(TEXT("SpawnInitialPosition"));
	this->SpawnInitialPosition->SetupAttachment(this->RootComponent);

	this->SpawnArea = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnArea"));
	this->SpawnArea->SetupAttachment(this->RootComponent);

	this->PrimaryActorTick.TickInterval = 0.1f;
}

// Called when the game starts or when spawned
void ARepeatSpawner::BeginPlay()
{
	Super::BeginPlay();
}

void ARepeatSpawner::Spawn()
{
	this->SpawnedActor = this->GetWorld()->SpawnActor<AActor>(this->ActorToSpawn.Get(), this->SpawnInitialPosition->GetComponentLocation(), FRotator::ZeroRotator);
	this->bIsWaitingToSpawn = false;
	this->ApplyTagsToSpawned();
	UPrimitiveComponent* CollisionComponent = Cast<UPrimitiveComponent>(this->SpawnedActor->GetRootComponent());
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);


	this->PlaySpawnShake();

	FTimerHandle AdjustPositionTimerHandle;
	this->GetWorld()->GetTimerManager().SetTimer(AdjustPositionTimerHandle, this, &ARepeatSpawner::AdjustPosition, this->AdjustPositionDelay);
}

void ARepeatSpawner::AdjustPosition()
{
	UPrimitiveComponent* CollisionComponent = Cast<UPrimitiveComponent>(this->SpawnedActor->GetRootComponent());
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	//this->SpawnedActor->SetActorLocation(this->SpawnAdjustedPosition->GetComponentLocation());
}

void ARepeatSpawner::PlaySpawnShake()
{
	UShakerComponent* Shaker = NewObject<UShakerComponent>(this->SpawnedActor);
	Shaker->RegisterComponent();
	Shaker->ShakeClass = this->SpawnShakeClass;
	Shaker->PlayShakeOnActor();
}

void ARepeatSpawner::TryStartSpawn()
{
	if (!this->bIsWaitingToSpawn)
	{
		this->bIsWaitingToSpawn = true;
		FTimerHandle SpawnTimerHandle;
		if (this->DelayAfterSpawnedActorMoved > 0)
		{
			this->GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &ARepeatSpawner::Spawn, this->DelayAfterSpawnedActorMoved);
		}
		else
		{
			this->Spawn();
		}
	}
}

void ARepeatSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	bool bShouldSpawn = false;
	if (this->SpawnTrigger == ESpawnTrigger::ON_DESTROYED)
	{
		bShouldSpawn = !IsValid(this->SpawnedActor);
	}
	else if (this->SpawnTrigger == ESpawnTrigger::ON_MOVED)
	{
		TSet<AActor*> OverlappingActors;
		this->SpawnArea->GetOverlappingActors(OverlappingActors, this->ActorToSpawn);
		bShouldSpawn = OverlappingActors.Num() == 0;
	}

	if (bShouldSpawn)
	{
		this->TryStartSpawn();
	}
}


void ARepeatSpawner::ApplyTagsToSpawned()
{
	if (IsValid(this->SpawnedActor))
	{
		for (FName Tag : this->Tags)
		{
			this->SpawnedActor->Tags.Add(Tag);
		}
	}
}
