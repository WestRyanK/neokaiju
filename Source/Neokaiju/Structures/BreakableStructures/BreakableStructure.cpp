// Fill out your copyright notice in the Description page of Project Settings.


#include "BreakableStructure.h"
#include "Neokaiju/Structures/BreakableStructures/BuildingBlocks/BuildingBlock.h"
#include "Neokaiju/Structures/BreakableStructures/BreakableStructureSubsystem.h"
#include "Components/SceneComponent.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Math/Box.h"
#include "Neokaiju/Core/SaveGame/NeokaijuSaveGame.h"
#include "Components/BoxComponent.h"

// Sets default values
ABreakableStructure::ABreakableStructure()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RootMesh"));
	RootComponent = Mesh;

	// Set up components
	//RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	//Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	//Mesh->SetupAttachment(RootComponent);
	//Mesh->SetRelativeLocation(FVector(0.0, 0.0, 0.0), false, nullptr, ETeleportType::TeleportPhysics);


	Mesh->SetConstraintMode(EDOFMode::XZPlane);
	Mesh->SetNotifyRigidBodyCollision(true);
	Mesh->SetLinearDamping(LinearDampingAmount);
	Mesh->SetAngularDamping(AngularDampingAmount);
	Mesh->OnComponentHit.AddDynamic(this, &ABreakableStructure::OnMeshHit);

	ScaleComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scale"));
	ScaleComponent->SetupAttachment(RootComponent);
	ScaleComponent->SetRelativeLocation(FVector(0.0, 0.0, 0.0), false, nullptr, ETeleportType::TeleportPhysics);

	RightBox = CreateDefaultSubobject<UBoxComponent>(TEXT("RightBox"));
	RightBox->SetupAttachment(ScaleComponent);
	RightBox->SetRelativeLocation(FVector(45.0, 0.0, 50.0), false, nullptr, ETeleportType::TeleportPhysics);
	RightBox->SetBoxExtent(FVector(13.0, 40.0, 40.0), true);

	LeftBox = CreateDefaultSubobject<UBoxComponent>(TEXT("LeftBox"));
	LeftBox->SetupAttachment(ScaleComponent);
	LeftBox->SetRelativeLocation(FVector(-45.0, 0.0, 50.0), false, nullptr, ETeleportType::TeleportPhysics);
	LeftBox->SetBoxExtent(FVector(13.0, 40.0, 40.0), true);

	TopBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TopBox"));
	TopBox->SetupAttachment(ScaleComponent);
	TopBox->SetRelativeLocation(FVector(0.0, 0.0, 95.0), false, nullptr, ETeleportType::TeleportPhysics);
	TopBox->SetBoxExtent(FVector(40.0, 40.0, 13.0), true);

	BottomBox = CreateDefaultSubobject<UBoxComponent>(TEXT("BottomBox"));
	BottomBox->SetupAttachment(ScaleComponent);
	BottomBox->SetRelativeLocation(FVector(0.0, 0.0, 5.0), false, nullptr, ETeleportType::TeleportPhysics);
	BottomBox->SetBoxExtent(FVector(40.0, 40.0, 13.0), true);

	AttachPosition = CreateDefaultSubobject<USceneComponent>(TEXT("AttachPosition"));
	AttachPosition->SetupAttachment(ScaleComponent);
	AttachPosition->SetRelativeLocation(FVector(0.0, 0.0, 50.0), false, nullptr, ETeleportType::TeleportPhysics);

	BlockTier = 1;

	AttributeSet = CreateDefaultSubobject<UBreakableStructureAttributeSet>(TEXT("AttributeSet"));
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>("AbilitySystemComponent");
}

void ABreakableStructure::OnConstruction(const FTransform& Transform)
{
	UpdateTier(BlockTier);

	if (this->UseAutoScaleToMesh)
	{
		this->ResizeBounds();
	}
}

void ABreakableStructure::UpdateTier(int Tier)
{
	if (TierGEList.IsValidIndex(Tier))
	{
		AbilitySystemComponent->ApplyGameplayEffectToSelf(TierGEList[Tier].GetDefaultObject(), 1, AbilitySystemComponent->MakeEffectContext());
	}
}



void ABreakableStructure::ResizeBounds()
{
	if (IsValid(Mesh) && IsValid(Mesh->GetStaticMesh()))
	{
		ScaleComponent->SetWorldScale3D(2 * Mesh->GetComponentScale() * Mesh->GetStaticMesh()->GetBoundingBox().GetExtent() / 100.0f);
		ScaleComponent->SetWorldRotation(Mesh->GetComponentRotation());
	}
}

// Called when the game starts or when spawned
void ABreakableStructure::BeginPlay()
{
	Super::BeginPlay();

	// Don't let physics be enabled
	this->SetIsSimulatingPhysics(false);

	UpdateTier(BlockTier);
	this->SetInitialEffects();

	this->ListenForAttributeChanges();

	this->UpdateOverlaps();
}

// Breakable Structures will need to initialize physics constraints so they fall with the blocks they are attached to
void ABreakableStructure::InitConstraint(UPhysicsConstraintComponent* Constraint, TSet<AActor*> NeighboringActors)
{
	for (AActor* Actor : NeighboringActors)
	{
		ABuildingBlock* NeighborBlock = Cast<ABuildingBlock>(Actor);

		if (IsValid(NeighborBlock))
		{
			// Other actor is a buildingblock

			if (NeighborBlock->ConstrainedNeighbors.Contains(this))
			{
				continue;
			}

			Constraint->SetConstrainedComponents(Mesh, FName(), NeighborBlock->Mesh, FName());
			Constraint->ConstraintActor2 = Actor;

			Constraint->SetLinearXLimit(ELinearConstraintMotion::LCM_Locked, 0);
			Constraint->SetLinearYLimit(ELinearConstraintMotion::LCM_Locked, 0);
			Constraint->SetLinearZLimit(ELinearConstraintMotion::LCM_Locked, 0);
			Constraint->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Locked, 0);
			Constraint->SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Locked, 0);
			Constraint->SetAngularTwistLimit(EAngularConstraintMotion::ACM_Locked, 0);

			Constraint->SetDisableCollision(true);
			//Constraint->SetLinearBreakable(true, 100000000);
			
			ConstrainedNeighbors.Add(NeighborBlock);
			return;
		}
	}
}


// Called every frame
void ABreakableStructure::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABreakableStructure::OnInteract()
{
}

UAbilitySystemComponent* ABreakableStructure::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ABreakableStructure::ListenForAttributeChanges()
{
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetHealthAttribute()).AddUObject(this, &ABreakableStructure::HealthChanged);
}

void ABreakableStructure::HealthChanged(const FOnAttributeChangeData& Data)
{
	//UE_LOG(LogTemp, Display, TEXT("Health Changed from %f to %f"), Data.OldValue, Data.NewValue);
	if (Data.NewValue <= 0)
	{
		DestroyStructure();
	}
	else
	{
		if (Data.NewValue < Data.OldValue)
		{
			if (!this->bIsLoadingState)
			{
				PlayOnDamagedFX(Data.OldValue, Data.NewValue);
			}
		}
		// Play on damaged fx if damaged.
	}
}


void ABreakableStructure::UpdateCharacterStats()
{
	// Get gamemode and update whatever is needed.
	return;
}

void ABreakableStructure::DestroyStructure()
{
	if (!this->bIsLoadingState && !this->bIsBreakableStructureBeingDestroyed)
	{
		this->bIsBreakableStructureBeingDestroyed = true;
		// Notify the BreakableStructureSubsystem that the structure was destroyed.
		// The BreakableStructureSubsystem broadcasts an event that a block was destroyed.
		// The KaijuScoreState listens to the BreakableStructureSubsystem's event to update score.
		UBreakableStructureSubsystem* BreakableStructureSubsystem = this->GetWorld()->GetSubsystem<UBreakableStructureSubsystem>();
		BreakableStructureSubsystem->StructureDestroyed(this);
		this->OnStructureDestroyed.Broadcast(this);

		PlayOnDestroyedFX();
		Destroy();
	}
}

void ABreakableStructure::OnMeshHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	return;
}



void ABreakableStructure::UpdateOverlaps()
{
	RightBox->UpdateOverlaps();
	LeftBox->UpdateOverlaps();
	TopBox->UpdateOverlaps();
	BottomBox->UpdateOverlaps();
}

bool ABreakableStructure::GetIsSimulatingPhysics() const
{
	return this->bIsSimulatingPhysics;
}

void ABreakableStructure::SetIsSimulatingPhysics(bool bIsSimulatingPhysicsValue)
{
	if (IsValid(this->Mesh))
	{
		this->bIsSimulatingPhysics = bIsSimulatingPhysicsValue;
		this->Mesh->SetSimulatePhysics(this->bIsSimulatingPhysics);
		this->Mesh->SetLinearDamping(0.1);
		this->Mesh->SetAngularDamping(0.2);

		if (bIsSimulatingPhysics)
		{
			this->OnStructureUnstable.Broadcast(this);
		}
	}
}

void ABreakableStructure::BeginLoadState()
{
	this->bIsLoadingState = true;
}

void ABreakableStructure::EndLoadState()
{
	this->bIsLoadingState = false;
}

void ABreakableStructure::SetInitialEffects()
{
	for (const TSubclassOf<UGameplayEffect> InitialEffect : this->InitialEffects)
	{
		this->AbilitySystemComponent->ApplyGameplayEffectToSelf(InitialEffect.GetDefaultObject(), 1, this->AbilitySystemComponent->MakeEffectContext());
	}
}
