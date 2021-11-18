// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingBlock.h"
#include "GameFramework/Actor.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "Neokaiju/Structures/BreakableStructures/BreakableStructureSubsystem.h"
#include "Neokaiju/Structures/BreakableStructures/BuildingBlocks/BlockManager.h"
#include "Neokaiju/KaijuCharacter/KaijuCharacter.h"
#include "Neokaiju/Ability/Core/EffectUtility.h"
#include "Components/BoxComponent.h"
#include "Components/TextRenderComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Containers/Array.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Materials/MaterialInstance.h"


// Sets default values
ABuildingBlock::ABuildingBlock()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set up components
	RightConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("RightConstraint"));
	RightConstraint->SetupAttachment(ScaleComponent);
	RightConstraint->SetRelativeLocation(FVector(50.0, 0.0, 50.0), false, nullptr, ETeleportType::TeleportPhysics);

	LeftConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("LeftConstraint"));
	LeftConstraint->SetupAttachment(ScaleComponent);
	LeftConstraint->SetRelativeLocation(FVector(-50.0, 0.0, 50.0), false, nullptr, ETeleportType::TeleportPhysics);

	BottomConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("BottomConstraint"));
	BottomConstraint->SetupAttachment(ScaleComponent);
	BottomConstraint->SetRelativeLocation(FVector(0.0, 0.0, 0.0), false, nullptr, ETeleportType::TeleportPhysics);

	TopConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("TopConstraint"));
	TopConstraint->SetupAttachment(ScaleComponent);
	TopConstraint->SetRelativeLocation(FVector(0.0, 0.0, 100.0), false, nullptr, ETeleportType::TeleportPhysics);

	TierIndicator = CreateDefaultSubobject<UTextRenderComponent>(TEXT("TierIndicator"));
	TierIndicator->SetupAttachment(RootComponent);
	TierIndicator->SetHiddenInGame(true);
	TierIndicator->SetAbsolute(false, false, true);
	TierIndicator->SetVerticalAlignment(EVerticalTextAligment::EVRTA_TextTop);
	//TierIndicator->SetRelativeLocation(FVector(-40, 60, 70));
	TierIndicator->SetRelativeRotation(FRotator(0, 90, 0));

	TypeIndicator = CreateDefaultSubobject<UTextRenderComponent>(TEXT("TypeIndicator"));
	TypeIndicator->SetupAttachment(RootComponent);
	TypeIndicator->SetAbsolute(false, false, true);
	//TypeIndicator->SetRelativeLocation(FVector(-40, 60, 10));
	TypeIndicator->SetRelativeRotation(FRotator(0, 90, 0));
	TypeIndicator->SetHiddenInGame(true);
}

// Called when the game starts or when spawned
void ABuildingBlock::BeginPlay()
{
	Super::BeginPlay();

	/*if (BlockType != BLOCK_BRITTLE)
	{
		InitNeighbors();
	}*/

	RightConstraint->SetDisableCollision(true);
	RightConstraint->SetAngularBreakable(bAngularBreakable, AngularLimit);
	RightConstraint->SetLinearBreakable(bLinearBreakable, LinearLimit);
	
	LeftConstraint->SetDisableCollision(true);
	LeftConstraint->SetAngularBreakable(bAngularBreakable, AngularLimit);
	LeftConstraint->SetLinearBreakable(bLinearBreakable, LinearLimit);

	BottomConstraint->SetDisableCollision(true);
	BottomConstraint->SetAngularBreakable(bAngularBreakable, AngularLimit);
	BottomConstraint->SetLinearBreakable(bLinearBreakable, LinearLimit);

	TopConstraint->SetDisableCollision(true);
	TopConstraint->SetAngularBreakable(bAngularBreakable, AngularLimit);
	TopConstraint->SetLinearBreakable(bLinearBreakable, LinearLimit);

	if (BlockType == BLOCK_INDEPENDENT)
	{
		if (IsValid(Mesh))
		{
			this->SetIsSimulatingPhysics(true);

			Mesh->SetConstraintMode(EDOFMode::Type::SixDOF);
			Mesh->GetBodyInstance()->bLockYTranslation = true;
			Mesh->GetBodyInstance()->SetDOFLock(EDOFMode::SixDOF);
		}
	}
	else
	{
		InitNeighbors();
	}
}

void ABuildingBlock::UpdateIndicators()
{
	if (!GIsCookerLoadingPackage) // Don't run this while cooking
	{
		if (IsValid(this->TierIndicator) && IsValid(this->TypeIndicator))
		{
			FColor TierColors[] = { FColor::White, FColor::Yellow, FColor::Orange, FColor::Magenta, FColor::Red, FColor::Green };
			this->TierIndicator->SetText(FText::FromString(FString::FromInt(this->BlockTier)));
			this->TierIndicator->SetTextRenderColor(TierColors[this->BlockTier]);

			FColor TypeColors[] = { FColor::Green, FColor::Blue, FColor::Red, FColor::Yellow, FColor::Purple };
			FString TypeTexts[] = { TEXT("S"), TEXT("B"), TEXT("F"), TEXT("I") };
			this->TypeIndicator->SetText(FText::FromString(TypeTexts[this->BlockType]));
			this->TypeIndicator->SetTextRenderColor(TypeColors[this->BlockType]);
		}
	}
}

// Called every frame
void ABuildingBlock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABuildingBlock::OnConstruction(const FTransform& Transform)
{
	FVector Min;
	FVector Max;

	BaseMat = UMaterialInstanceDynamic::Create(BaseMatSource, this);
	WindowMat = UMaterialInstanceDynamic::Create(WindowMatSource, this);
	if (IsValid(Mesh))
	{
		Mesh->SetMaterial(0, BaseMat);
		Mesh->SetMaterial(1, WindowMat);

		Mesh->GetLocalBounds(Min, Max);
		TierIndicator->SetRelativeLocation(FVector(Min.X, Max.Y + 1.0, Max.Z));
		TypeIndicator->SetRelativeLocation(FVector(Min.X, Max.Y + 1.0, Min.Z));
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("Block initialized with null mesh!"));
	}

	this->UpdateIndicators();

	
}

void ABuildingBlock::HealthChanged(const FOnAttributeChangeData& Data)
{
	ABreakableStructure::HealthChanged(Data);
	UpdateCracks();
}

// Getters/Setters 

TEnumAsByte<BuildingBlockType> ABuildingBlock::GetBlockType()
{
	return BlockType;
}


void ABuildingBlock::SetPhysics()
{
	// Don't set for foundations, brittle is destroyed

	if (BlockType == BLOCK_BRITTLE)
	{
		DestroyStructure();
	}
	else if (BlockType == BLOCK_STICKY)
	{
		this->SetIsSimulatingPhysics(true);
		//this->OnStructureUnstable.Broadcast(this);
	}
	else if (BlockType == BLOCK_INDEPENDENT)
	{
		// do nothing.
	}
}

void ABuildingBlock::OnMeshHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	AKaijuCharacter* KaijuCharacter = Cast<AKaijuCharacter>(OtherActor);
	ABuildingBlock* OtherBlock = Cast<ABuildingBlock>(OtherActor);
	float TierDamageMultiplier = 1.0f;
	//if (InteractableTags.HasTag(FGameplayTag::RequestGameplayTag(FName("State.Thrown"))))
	//{
	//	UE_LOG(LogTemp, Display, TEXT("Thrown"));
	//	// Make Thrown blocks multiply the impulse damage relative to their tier
	//	if (BlockTier > 0)
	//	{
	//		TierDamageMultiplier = ThrowDamageTierMultiplier * BlockTier;
	//	}
	//	else
	//	{
	//		// If it's a Tier 0 block, we don't want to multiply in the tier because that would cause it to do no damage
	//		TierDamageMultiplier = ThrowDamageTierMultiplier;
	//	}
	//	InteractableTags.RemoveTag(FGameplayTag::RequestGameplayTag(FName("State.Thrown")));
	//}

	if (!Neighbors.Contains(OtherBlock) &&
		NormalImpulse.Size() > this->MinHitImpulseToDamage&&
		this->ImpulseDamageStructureEffect)
	{
		//UE_LOG(LogTemp, Display, TEXT("Hit Impulse: %f"), NormalImpulse.Size());
		float ImpulseDamage = NormalImpulse.Size() * this->HitImpulseDamageMultiplier * -1.0f;
		//UE_LOG(LogTemp, Display, TEXT("Hit Impulse Level: %f"), ImpulseDamage);
		UGameplayEffect* ImpulseEffect = this->ImpulseDamageStructureEffect.GetDefaultObject();
		FGameplayEffectSpec Spec = FGameplayEffectSpec(ImpulseEffect, this->AbilitySystemComponent->MakeEffectContext(), 1);


		if (!IsValid(OtherBlock)) // If the other actor isn't a block, it's not going to damage you, so damage yourself.
		{
			Spec.SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(TEXT("Caller.Damage")), ImpulseDamage);
			this->AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(Spec);
			//UE_LOG(LogTemp, Display, TEXT("Hit yourself"));
		}

		// If the other actor can be damaged, damage it!
		TScriptInterface<IAbilitySystemInterface> ASCInterface = OtherActor;
		if (ASCInterface && !IsValid(KaijuCharacter))
		{
			//UE_LOG(LogTemp, Display, TEXT("Hit other with damage level %f"), ImpulseDamage * TierDamageMultiplier);
			UAbilitySystemComponent* OtherASC = ASCInterface->GetAbilitySystemComponent();

			Spec.SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(TEXT("Caller.Damage")), ImpulseDamage * TierDamageMultiplier);
			OtherASC->ApplyGameplayEffectSpecToSelf(Spec);
		}
	}
}


void ABuildingBlock::DestroyStructure()
{
	// Remove self from neighbors' lists of neighbors
	if (!this->bIsBuildingBlockBeingDestroyed)
	{
		this->bIsBuildingBlockBeingDestroyed = true;

		for (ABuildingBlock* NeighborBlock : Neighbors)
		{
			if (IsValid(NeighborBlock) && IsValid(this))
			{
				NeighborBlock->Neighbors.Remove(this);
				NeighborBlock->UpdateDeform(this);
			}
		}

		UBreakableStructureSubsystem* BreakableStructureSubsystem = this->GetWorld()->GetSubsystem<UBreakableStructureSubsystem>();
		BreakableStructureSubsystem->BlockManager->CheckBlocks(Neighbors);

		// Finish
		Super::DestroyStructure();
	}
}

void ABuildingBlock::InitNeighbors()
{
	// they don't need that nonsense

	TSet<AActor*> RightGroup;
	TSet<AActor*> LeftGroup;
	TSet<AActor*> TopGroup;
	TSet<AActor*> BottomGroup;

	RightBox->GetOverlappingActors(RightGroup, ABuildingBlock::StaticClass());
	LeftBox->GetOverlappingActors(LeftGroup, ABuildingBlock::StaticClass());
	TopBox->GetOverlappingActors(TopGroup, ABuildingBlock::StaticClass());
	BottomBox->GetOverlappingActors(BottomGroup, ABuildingBlock::StaticClass());

	RightNeighbors = TSet<AActor*>(RightGroup);
	LeftNeighbors = TSet<AActor*>(LeftGroup);
	TopNeighbors = TSet<AActor*>(TopGroup);
	BottomNeighbors = TSet<AActor*>(BottomGroup);

	RightGroup.Remove(this);
	LeftGroup.Remove(this);
	TopGroup.Remove(this);
	BottomGroup.Remove(this);

	// Add to neighbors

	// Right
	if (AttachRight)
	{
		for (AActor* Actor : RightGroup)
		{
			AddToNeighborsIfBuildingBlock(Actor);
		}
	}
	// Left
	for (AActor* Actor : LeftGroup)
	{
		AddToNeighborsIfBuildingBlock(Actor);
	}
	//Top
	for (AActor* Actor : TopGroup)
	{
		AddToNeighborsIfBuildingBlock(Actor);
	}
	// Bottom
	if (AttachBottom)
	{
		for (AActor* Actor : BottomGroup)
		{
			AddToNeighborsIfBuildingBlock(Actor);
		}
	}

	// Initialize physics constraints
	InitConstraint(RightConstraint, RightGroup);
	InitConstraint(BottomConstraint, BottomGroup);
	InitConstraint(LeftConstraint, LeftGroup);
	InitConstraint(TopConstraint, TopGroup);


	//UE_LOG(LogTemp, Display, TEXT("%s Neighbors: "), *this->GetName());
	//for (ABreakableStructure* Neighbor : Neighbors)
	//{
	//	UE_LOG(LogTemp, Display, TEXT("%s"), *Neighbor->GetName());
	//}
}

void ABuildingBlock::AddToNeighborsIfBuildingBlock(AActor* Actor)
{
	ABuildingBlock* Neighbor = Cast<ABuildingBlock>(Actor);
	if (IsValid(Neighbor))
	{
		if (Neighbor != this && Neighbor->BlockType != BLOCK_INDEPENDENT)
		{

			Neighbors.AddUnique(Neighbor);
		}
	}
}


void ABuildingBlock::Grab_Implementation(USceneComponent* Parent, FName SocketName, FVector GrabLocation, bool GrabGroup)
{
	UBreakableStructureSubsystem* BreakableStructureSubsystem = this->GetWorld()->GetSubsystem<UBreakableStructureSubsystem>();

	if (IsValid(Mesh))
	{
		this->SetIsSimulatingPhysics(false);
		this->OnStructureUnstable.Broadcast(this);
		Mesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
		this->SetActorLocation(Parent->GetSocketLocation(SocketName) + (GetActorLocation() - GrabLocation));

		FAttachmentTransformRules TransformRules = FAttachmentTransformRules(EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, true);
		//FAttachmentTransformRules TransformRules = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, true);
	
		Mesh->AttachToComponent(Parent, TransformRules, SocketName);

		UE_LOG(LogTemp, Display, TEXT("Before grab, Grabbed block had %d Neighbors"), Neighbors.Num());
		// remove the block from its neighbors so they don't hold it back
		for (ABuildingBlock* NeighborBlock : Neighbors)
		{
			if (IsValid(NeighborBlock) && IsValid(this))
			{
				NeighborBlock->Neighbors.Remove(this);
			}
		}

		//disconnect the block from its neighbors if the whole structure can't move or if you're only grabbing one
		if (!BreakableStructureSubsystem->BlockManager->CanEnablePhysics(Neighbors) || !GrabGroup)
		{

			Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			BreakableStructureSubsystem->BlockManager->CheckBlocks(Neighbors);
			for (ABuildingBlock* NeighborBlock : Neighbors)
			{
				if (IsValid(NeighborBlock) && IsValid(this))
				{

					NeighborBlock->UpdateDeform(this);
					UpdateDeform(NeighborBlock);
				}
			}
			Neighbors.Empty();

		}
		else
		{
			// enable physics if possible
			BreakableStructureSubsystem->BlockManager->CheckBlocks(Neighbors);
		}


		UE_LOG(LogTemp, Display, TEXT("Grabbed block now has %d Neighbors"), Neighbors.Num());

	}
}

bool ABuildingBlock::CanBeGrabbed_Implementation(AKaijuCharacter* Kaiju)
{
	return true;
}

void ABuildingBlock::Throw_Implementation(FVector Direction, float LaunchVelocity, TSubclassOf<UGameplayEffect> ThrownObjectEffect)
{
	//Let go
	FDetachmentTransformRules TransformRules = FDetachmentTransformRules(EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, true);
	Mesh->DetachFromComponent(TransformRules);
	this->SetIsSimulatingPhysics(true);
	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	// make sure the grabbed block is in its neighbors' lists and launch each neighbor
	for (ABuildingBlock* NeighborBlock : Neighbors)
	{
		if (IsValid(NeighborBlock) && IsValid(this))
		{
			if (IsValid(ThrownObjectEffect))
			{
				UEffectUtility::ApplyEffectToActor(NeighborBlock, ThrownObjectEffect);
			}

			NeighborBlock->Neighbors.AddUnique(this);
			NeighborBlock->Mesh->SetPhysicsLinearVelocity(Direction * LaunchVelocity, false);
		}
	}

	// launch
	Mesh->SetPhysicsLinearVelocity(Direction * LaunchVelocity, false);
}

void ABuildingBlock::UpdateDeform(AActor* Neighbor)
{
	FName Side;

	if (RightNeighbors.Contains(Neighbor))
	{
		BaseMat->SetScalarParameterValue("XPos", 1.0f);
	}

	if (LeftNeighbors.Contains(Neighbor))
	{
		BaseMat->SetScalarParameterValue("XNeg", 1.0f);
	}

	if (TopNeighbors.Contains(Neighbor))
	{
		BaseMat->SetScalarParameterValue("ZPos", 1.0f);
	}

	if (BottomNeighbors.Contains(Neighbor))
	{
		BaseMat->SetScalarParameterValue("ZNeg", 1.0f);
	}
}

void ABuildingBlock::UpdateCracks()
{
	float Percent = AttributeSet->GetHealth() / AttributeSet->GetMaxHealth();
	float Crack1, Crack2, Crack3;

	if (Percent < 0.375f)
	{
		Crack1 = 1.0f;
		Crack2 = 1.0f;
		Crack3 = 1.0f;
	}
	else if (Percent < 0.625f)
	{
		Crack1 = 1.0f;
		Crack2 = 1.0f;
		Crack3 = 0.0f;
	}
	else if (Percent < 0.875)
	{
		Crack1 = 1.0f;
		Crack2 = 0.0f;
		Crack3 = 0.0f;
	}
	else
	{
		Crack1 = 0.0f;
		Crack2 = 0.0f;
		Crack3 = 0.0f;
	}

	BaseMat->SetScalarParameterValue("Crack1_ON", Crack1);
	BaseMat->SetScalarParameterValue("Crack2_ON", Crack2);
	BaseMat->SetScalarParameterValue("Crack3_ON", Crack3);
	WindowMat->SetScalarParameterValue("Crack1_ON", Crack1);
	WindowMat->SetScalarParameterValue("Crack2_ON", Crack2);
	WindowMat->SetScalarParameterValue("Crack3_ON", Crack3);
}