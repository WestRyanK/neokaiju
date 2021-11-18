// Fill out your copyright notice in the Description page of Project Settings.


#include "Prop.h"
#include "Neokaiju/Structures/BreakableStructures/BreakableStructureAttributeSet.h"
#include "Components/BoxComponent.h"
#include "Neokaiju/Structures/BreakableStructures/BuildingBlocks/BuildingBlock.h"
#include "Neokaiju/KaijuCharacter/KaijuCharacter.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "Engine.h"

// Sets default values
AProp::AProp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set up component
	RightConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("RightConstraint"));
	RightConstraint->SetupAttachment(ScaleComponent);

	LeftConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("LeftConstraint"));
	LeftConstraint->SetupAttachment(ScaleComponent);

	TopConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("TopConstraint"));
	TopConstraint->SetupAttachment(ScaleComponent);

	BottomConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("BottomConstraint"));
	BottomConstraint->SetupAttachment(ScaleComponent);

	// If we add attached blocks this will be set to true
	HasAttachedBlocks = false;
}

// Called when the game starts or when spawned
void AProp::BeginPlay()
{
	Super::BeginPlay();

	InitAttachedBlocks();

	if (AttachedBlocks.Num() > 0)
	{
		HasAttachedBlocks = true;
		for (ABreakableStructure* Block : AttachedBlocks)
		{
			if (IsValid(Block))
			{
				Block->OnDestroyed.AddUniqueDynamic(this, &AProp::AttachedBlockDestroyed);
				Block->OnStructureUnstable.AddUniqueDynamic(this, &AProp::AttachedBlockUnstable);
			}
		}
	}

	Mesh->OnComponentHit.AddDynamic(this, &AProp::OnHit);
	Mesh->OnComponentBeginOverlap.AddDynamic(this, &AProp::OnOverlap);
}

void AProp::BeginDestroy()
{
	for (ABreakableStructure* Block : AttachedBlocks)
	{
		if (IsValid(Block))
		{
			Block->OnDestroyed.RemoveAll(this);
			Block->OnStructureUnstable.RemoveAll(this);
		}
	}

	Super::BeginDestroy();
}

// Called every frame
void AProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProp::AttachedBlockUnstable(ABreakableStructure* UnstableStructure)
{
	ABuildingBlock* UnstableBlock = Cast<ABuildingBlock>(UnstableStructure);
	if (IsValid(UnstableBlock))
	{
		UE_LOG(LogTemp, Display, TEXT("%s: Attached block %s is unstable"), *this->GetName(), *UnstableBlock->GetName());

		// Fall with blocks if they are all simulating physics. Otherwise, release constraint and remove this block from attached blocks
		for (ABreakableStructure* AttachedBlock : AttachedBlocks)
		{
			ABuildingBlock* AttachedBuildingBlock = Cast<ABuildingBlock>(AttachedBlock);
			if (IsValid(AttachedBuildingBlock))
			{
				if (!AttachedBlock->GetIsSimulatingPhysics())
				{
					if (ReleaseConstraint(UnstableBlock))
					{
						UE_LOG(LogTemp, Warning, TEXT("%s: Released constraint to %s"), *this->GetName(), *UnstableBlock->GetName());
						AttachedBlocks.Remove(UnstableBlock);
						return;
					}
					else
					{
						UE_LOG(LogTemp, Warning, TEXT("%s: Unable to remove physics constraint from unstable attached block"), *this->GetName());
						return;
					}
				}
			}
			
		}
		UE_LOG(LogTemp, Display, TEXT("%s: Simulating Physics"), *this->GetName());
		this->SetIsSimulatingPhysics(true);
	}
}

void AProp::AttachedBlockDestroyed(AActor* DestroyedActor)
{
	
	ABreakableStructure* DestroyedBlock = Cast<ABreakableStructure>(DestroyedActor);
	if (IsValid(DestroyedBlock))
	{
		UE_LOG(LogTemp, Display, TEXT("%s: Attached block %s was destroyed"), *this->GetName(), *DestroyedBlock->GetName());
		// Might remove automatically, but if not destroyed yet remove manually
		if (AttachedBlocks.Contains(DestroyedBlock))
		{
			AttachedBlocks.Remove(DestroyedBlock);
		}

		// Simulate physics if not attached to any more blocks
		if (AttachedBlocks.Num() == 0)
		{
			UE_LOG(LogTemp, Display, TEXT("%s: Destroyed"), *this->GetName());

			this->DestroyStructure();
			//this->SetIsSimulatingPhysics(true);
		}
	}
	
}

bool AProp::GetHasAttachedBlocks()
{
	return HasAttachedBlocks;
}


void AProp::InitAttachedBlocks()
{
	TSet<AActor*> OverlappingActorsR;
	TSet<AActor*> OverlappingActorsL;
	TSet<AActor*> OverlappingActorsT;
	TSet<AActor*> OverlappingActorsB;

	RightBox->GetOverlappingActors(OverlappingActorsR, ABreakableStructure::StaticClass());
	LeftBox->GetOverlappingActors(OverlappingActorsL, ABreakableStructure::StaticClass());
	TopBox->GetOverlappingActors(OverlappingActorsT, ABreakableStructure::StaticClass());
	BottomBox->GetOverlappingActors(OverlappingActorsB, ABreakableStructure::StaticClass());

	// Right
	for (AActor* Actor : OverlappingActorsR)
	{
		AddToAttachedBlocksIfBuildingBlock(Actor);
	}
	// Left
	for (AActor* Actor : OverlappingActorsL)
	{
		AddToAttachedBlocksIfBuildingBlock(Actor);
	}
	//Top
	for (AActor* Actor : OverlappingActorsT)
	{
		AddToAttachedBlocksIfBuildingBlock(Actor);
	}
	// Bottom
	for (AActor* Actor : OverlappingActorsB)
	{
		AddToAttachedBlocksIfBuildingBlock(Actor);
	}

	// Initialize physics constraints
	InitConstraint(RightConstraint, OverlappingActorsR);
	InitConstraint(LeftConstraint, OverlappingActorsL);
	InitConstraint(TopConstraint, OverlappingActorsT);
	InitConstraint(BottomConstraint, OverlappingActorsB);
}

void AProp::AddToAttachedBlocksIfBuildingBlock(AActor* Actor)
{
	ABreakableStructure* Block = Cast<ABreakableStructure>(Actor);
	if (IsValid(Block))
	{
		UE_LOG(LogTemp, Display, TEXT("%s: Attached to block %s"), *this->GetName(), *Block->GetName());
		AttachedBlocks.AddUnique(Block);
	}
}

bool AProp::ReleaseConstraint(ABreakableStructure* AttachedBlock)
{
	// Check right constraint
	ABreakableStructure* AnAttachedBlock = Cast<ABreakableStructure>(RightConstraint->ConstraintActor2);
	if (IsValid(AnAttachedBlock))
	{
		if (AnAttachedBlock == AttachedBlock)
		{
			RightConstraint->BreakConstraint();
			RightConstraint->SetDisableCollision(false);
			return true;
		}
	}
	// Check left constraint
	AnAttachedBlock = Cast<ABreakableStructure>(LeftConstraint->ConstraintActor2);
	if (IsValid(AnAttachedBlock))
	{
		if (AnAttachedBlock == AttachedBlock)
		{
			LeftConstraint->BreakConstraint();
			LeftConstraint->SetDisableCollision(false);
			return true;
		}
	}
	// Check top constraint
	AnAttachedBlock = Cast<ABreakableStructure>(TopConstraint->ConstraintActor2);
	if (IsValid(AnAttachedBlock))
	{
		if (AnAttachedBlock == AttachedBlock)
		{
			TopConstraint->BreakConstraint();
			TopConstraint->SetDisableCollision(false);
			return true;
		}
	}
	// Check bottom constraint
	AnAttachedBlock = Cast<ABreakableStructure>(BottomConstraint->ConstraintActor2);
	if (IsValid(AnAttachedBlock))
	{
		if (AnAttachedBlock == AttachedBlock)
		{
			BottomConstraint->BreakConstraint();
			BottomConstraint->SetDisableCollision(false);
			return true;
		}
	}

	// None of the constraints found the attached block
	return false;
}

void AProp::OnHit(
	UPrimitiveComponent* HitComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComponent,
	FVector NormalImpulse,
	const FHitResult& Hit)
{
	ABuildingBlock* Block = Cast<ABuildingBlock>(OtherActor);
	if (Block && !AttachedBlocks.Contains(Block))
	{
		this->DestroyStructure();
		return;
	}

	AProp* Prop = Cast<AProp>(OtherActor);
	if (Prop && !AttachedBlocks.Contains(Prop))
	{
		this->DestroyStructure();
		return;
	}
}

void AProp::OnOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	AKaijuCharacter* Character = Cast<AKaijuCharacter>(OtherActor);
	if (Character)
	{
		this->DestroyStructure();
	}
}
