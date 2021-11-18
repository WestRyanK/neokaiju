// Fill out your copyright notice in the Description page of Project Settings.


#include "Claw.h"
#include "Components/PrimitiveComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Neokaiju/Structures/BreakableStructures/BuildingBlocks/BuildingBlock.h"
#include "Neokaiju/KaijuCharacter/KaijuCharacter.h"
#include "Neokaiju/Ability/Core/Abilities/BaseGameplayAbility.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"


// Sets default values
AClaw::AClaw()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	this->SetActorTickEnabled(true);

	this->ActiveArea = CreateDefaultSubobject<UBoxComponent>(TEXT("ActiveArea"));
	this->ActiveArea->SetupAttachment(RootComponent);
	this->ActiveArea->SetRelativeLocation(FVector(0.0, 0.0, -400.0), false, nullptr, ETeleportType::TeleportPhysics);
	this->ActiveArea->SetBoxExtent(FVector(50.0, 50.0, 50.0), false);

	this->KaijuTargetArea = CreateDefaultSubobject<UBoxComponent>(TEXT("KaijuTargetArea"));
	this->KaijuTargetArea->SetupAttachment(RootComponent);
	this->KaijuTargetArea->SetRelativeLocation(FVector(0.0, 0.0, -400.0), false, nullptr, ETeleportType::TeleportPhysics);

	this->CraneBoxComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CraneBox"));
	this->CraneBoxComponent->SetupAttachment(RootComponent);
	this->CraneBoxComponent->SetRelativeLocation(FVector(0.0, 0.0, -60.0), false, nullptr, ETeleportType::TeleportPhysics);

	this->AttachBox->SetupAttachment(CraneBoxComponent);
	this->AttachBox->SetRelativeLocation(FVector(0.0, 0.0, 50.0), false, nullptr, ETeleportType::TeleportPhysics);

	this->CheckLeftHitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CheckLeftHitBox"));
	this->CheckLeftHitBox->SetupAttachment(AttachBox);
	this->CheckLeftHitBox->SetRelativeLocation(FVector(-200.0, 0.0, 0.0), false, nullptr, ETeleportType::TeleportPhysics);
	this->CheckLeftHitBox->SetBoxExtent(FVector(25.0, 25.0, 5.0), true);

	this->CheckRightHitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CheckRightHitBox"));
	this->CheckRightHitBox->SetupAttachment(AttachBox);
	this->CheckRightHitBox->SetRelativeLocation(FVector(200.0, 0.0, 0.0), false, nullptr, ETeleportType::TeleportPhysics);
	this->CheckRightHitBox->SetBoxExtent(FVector(25.0, 25.0, 5.0), true);

	this->ClawMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ClawMesh"));
	this->ClawMesh->SetupAttachment(CraneBoxComponent);
	this->ClawMesh->SetRelativeLocation(FVector(0.0, 0.0, -50.0), false, nullptr, ETeleportType::TeleportPhysics);

	this->ClawHitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("ClawHitBox"));
	this->ClawHitBox->SetupAttachment(ClawMesh);
	this->ClawHitBox->SetRelativeLocation(FVector(0.0, 0.0, -130.0), false, nullptr, ETeleportType::TeleportPhysics);

	this->MoveClawComponent = CreateDefaultSubobject<UMoveClawComponent>(TEXT("MoveClawComponent"));
}


// Called when the game starts or when spawned
void AClaw::BeginPlay()
{
	Super::BeginPlay();

	// Set movement curve
	if (!this->ClawMovementCurve)
	{
		UE_LOG(LogTemp, Display, TEXT("ERROR: Claw self destructed because no movement curve."));

		if (IsValid(this->BB_CraneBox))
		{
			this->BB_CraneBox->DestroyStructure();
		}
		this->DestroyAttachment();
	}

	InitActiveLimits();

	this->ActiveArea->OnComponentBeginOverlap.AddUniqueDynamic(this, &AClaw::OnActiveAreaBeginOverlap);
	this->ActiveArea->OnComponentEndOverlap.AddUniqueDynamic(this, &AClaw::OnActiveAreaEndOverlap);
	this->KaijuTargetArea->OnComponentBeginOverlap.AddUniqueDynamic(this, &AClaw::OnKaijuTargetAreaBeginOverlap);
	this->KaijuTargetArea->OnComponentEndOverlap.AddUniqueDynamic(this, &AClaw::OnKaijuTargetAreaEndOverlap);

	this->ClawHitBox->OnComponentBeginOverlap.AddUniqueDynamic(this, &AClaw::OnClawHitBoxBeginOverlap);

	this->MoveClawComponent->ReachedEndEvent.AddUniqueDynamic(this, &AClaw::OnClawReachedEnd);
}

// Called every frame
void AClaw::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (this->Active)
	{
		CheckAttachedBlock();

		if (!this->PausedForAnimation) 
		{
			if (this->Tracking)
			{
				this->Track(DeltaTime);
			}
			else if (!Raising && !Lowering)
			{
				if (this->CaughtBlock || this->CaughtCharacter)
				{
					this->CarryObjective(DeltaTime);
				}
			}

			if (this->Timer > this->TimeToCatch)
			{

				// Catch Kaiju
				this->Timer = 0.0f;

				this->Lowering = true;
				this->Tracking = false;
				this->PlayOnClawExtendFX();

				// Lower claw
				FVector End = FVector(this->ClawMesh->GetComponentLocation().X, this->ClawMesh->GetComponentLocation().Y, this->LowerLimit);
				this->MoveClawComponent->MoveClaw(this->ClawMovementCurve, this->ClawMesh, End, this->ClawLoweringSpeed, true);
			}
		}
	}
}

void AClaw::AttachedBlockUnstable(ABreakableStructure* UnstableStructure)
{
	// Original attached block might not be our block anymore. Do nothing.
}

void AClaw::AttachedBlockDestroyed(AActor* DestroyedActor)
{
	// Original attached block might not be our block anymore. Do nothing.
}

void AClaw::DestroyClaw()
{
	// Spawn a building block version of the crane box and the claw so that we will have building block fx
	this->CraneBoxComponent->SetVisibility(false, false);
	this->ClawMesh->SetVisibility(false, false);
	FActorSpawnParameters SpawnInfo;
	this->BB_CraneBox = GetWorld()->SpawnActor<ABuildingBlock>(ABuildingBlock::StaticClass(), this->CraneBoxComponent->GetComponentTransform(), SpawnInfo);
	ABuildingBlock* BB_Claw = GetWorld()->SpawnActor<ABuildingBlock>(ABuildingBlock::StaticClass(), this->ClawMesh->GetComponentTransform(), SpawnInfo);

	/*FAttachmentTransformRules FATR = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, false);
	this->BB_CraneBox->AttachToComponent(this->CraneBoxComponent, FATR);*/

	if (IsValid(this->BB_CraneBox) && IsValid(BB_Claw))
	{
		//this->BB_CraneBox->OnDestroyed.AddUniqueDynamic(this, &AClaw::OnCraneBoxDestroyed);
		//this->BB_CraneBox->UpdateTier(this->CraneBoxTier);

		// CraneBox stand in
		if (this->CraneBoxComponent->GetStaticMesh())
		{
			this->BB_CraneBox->Mesh->SetStaticMesh(this->CraneBoxComponent->GetStaticMesh());
			if (this->CraneBoxComponent->GetMaterial(0))
			{
				this->BB_CraneBox->Mesh->SetMaterial(0, this->CraneBoxComponent->GetMaterial(0));
			}
		}
		else
		{
			UE_LOG(LogTemp, Display, TEXT("ERROR: Claw was not given a stand in static mesh or material for its crane box"));
		}

		// Claw stand in 
		if (this->ClawStaticMesh)
		{
			BB_Claw->Mesh->SetStaticMesh(this->ClawStaticMesh);
			if (this->ClawMesh->GetMaterial(0))
			{
				BB_Claw->Mesh->SetMaterial(0, this->ClawMesh->GetMaterial(0));
			}
		}
		else
		{
			UE_LOG(LogTemp, Display, TEXT("ERROR: Claw was not given a stand in static mesh or material for its claw"));
		}

		// Destroy the stand in building blocks
		this->BB_CraneBox->DestroyStructure();
		BB_Claw->DestroyStructure();
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("ERROR: Claw could not spawn crane box or claw building block for destruction"));
		this->DestroyAttachment();
	}

	this->DestroyAttachment();
}

void AClaw::OnActiveAreaBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AKaijuCharacter* CheckCharacter = Cast<AKaijuCharacter>(OtherActor);
	if (IsValid(CheckCharacter))
	{
		UE_LOG(LogTemp, Display, TEXT("Crane Active"));
		this->Character = CheckCharacter;
		this->Active = true;
		this->Tracking = true;
		if (!Lowering && !Raising)
		{
			if (this->MoveBlocks)
			{
				StartNewObjective(); // Should be block objective 
			}
		}
	}
}

void AClaw::OnActiveAreaEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (this->Active) // Only care if character has entered and not left yet
	{
		AKaijuCharacter* CheckCharacter = Cast<AKaijuCharacter>(OtherActor);
		if (IsValid(CheckCharacter))
		{
			UE_LOG(LogTemp, Display, TEXT("Crane End Active"));
			this->Timer = 0.0f;
			this->Active = false;
			this->Tracking = false;
			this->CurrentObjective = nullptr;
			this->Character = nullptr;
		}
	}
}

void AClaw::OnKaijuTargetAreaBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (this->Active && !this->CaughtCharacter) // Don't count if we carried character to edge of area
	{
		AKaijuCharacter* CheckCharacter = Cast<AKaijuCharacter>(OtherActor);
		if (IsValid(CheckCharacter))
		{
			this->KaijuInTarget = true;
			if (!Lowering && !Raising)
			{
				StartNewObjective(); // Should be character objective
			}
		}
	}
}

void AClaw::OnKaijuTargetAreaEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (this->Active && !this->CaughtCharacter) // Don't count if we carried character to edge of area
	{
		if (this->KaijuInTarget)
		{
			AKaijuCharacter* CheckCharacter = Cast<AKaijuCharacter>(OtherActor);
			if (IsValid(CheckCharacter))
			{
				this->KaijuInTarget = false;
				if (!Lowering && !Raising) // We are raising, or lowering and will raise after. Tracking will be set true after we are done raising
				{
					if (this->MoveBlocks)
					{
						StartNewObjective(); // Should be a block objective
					}
				}
			}
		}
	}
}

void AClaw::OnClawHitBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Dont go through character no matter what. Stop if we hit a mesh.
	UMeshComponent* HitMeshComponent = Cast<UMeshComponent>(OtherComp);
	AKaijuCharacter* CheckCharacter = Cast<AKaijuCharacter>(OtherActor);
	UE_LOG(LogTemp, Display, TEXT("Claw hit: %s %s with %s"), *OtherActor->GetName(), *OtherComp->GetName(), *OverlappedComp->GetName());
	if (HitMeshComponent || CheckCharacter)
	{
		UE_LOG(LogTemp, Display, TEXT("Claw Hit"));
		if (this->Lowering && !this->CaughtCharacter) // Only catch while lowering
		{
			// We either hit something or have character/block objective, so make this end of route
			this->MoveClawComponent->Stop(); // Will call this->OnClawReachedEnd().

			if (IsValid(this->CurrentObjective)) // See if we should pick up character or block
			{
				if (IsValid(this->Character) && (this->CurrentObjective == this->Character))
				{
					if (OtherActor == this->Character)
					{
						//UE_LOG(LogTemp, Display, TEXT("CLAW CAUGHT CHARACTER"));

						// Send gameplay event with capture ability tag to character, applying the capture ability to it
						bool bWasSuccessful = UCaptureAbility::CaptureActor(this->CaptureAbility, this, this->Character, this->NumStrugglesToEscapeCapture);
						if (bWasSuccessful)
						{
							this->CaughtCharacter = true;
							this->Tracking = false;
							PlayOnCaptureFX();
						}
					}
				}
				else if (this->MoveBlocks)
				{
					ABuildingBlock* HitBlock = Cast<ABuildingBlock>(OtherActor);
					if (IsValid(HitBlock))
					{
						if (HitBlock == this->CurrentObjective)
						{
							//UE_LOG(LogTemp, Display, TEXT("GRABBING BLOCK"));

							HitBlock->SetIsSimulatingPhysics(false);
							bool bWasSuccessful = UCaptureAbility::CaptureActor(this->CaptureAbility, this, this->CurrentObjective, 3);
							if (bWasSuccessful)
							{
								this->CaughtBlock = true;
								this->Tracking = false;
							}
						}
					}
				}
			}
		}
	}
}

void AClaw::OnClawReachedEnd()
{
	if (this->Lowering)
	{
		if (this->CaughtBlock)
		{
			// We are dropping a block
			this->DropBlock();
		}
		FVector End = FVector(this->ClawMesh->GetComponentLocation().X, this->ClawMesh->GetComponentLocation().Y, this->InitialClawHeight);
		this->Lowering = false;
		this->Raising = true;
		this->MoveClawComponent->MoveClaw(this->ClawMovementCurve, this->ClawMesh, End, this->ClawRaisingSpeed, false);
	}
	else
	{
		if (this->CaughtCharacter)
		{
			// We have lifted character
			// Move character somewhere?
			// Move to farthest end
			this->LocationObjective = (abs(this->CraneBoxComponent->GetComponentLocation().X - this->RightLimit) > abs(this->CraneBoxComponent->GetComponentLocation().X - this->LeftLimit)) ?
				FVector(this->RightLimit, 0.0f, 0.0f) : FVector(this->LeftLimit, 0.0f, 0.0f);
		}
		else if (this->CaughtBlock)
		{
			// We have lifted block
			// Move somewhere
			// move to farthest end
			this->LocationObjective = (abs(this->CraneBoxComponent->GetComponentLocation().X - this->RightLimit) > abs(this->CraneBoxComponent->GetComponentLocation().X - this->LeftLimit)) ?
				FVector(this->RightLimit, 0.0f, 0.0f) : FVector(this->LeftLimit, 0.0f, 0.0f);
		}
		else
		{
			// Done raising claw, start new objective
			StartNewObjective();
		}
		this->Raising = false;
	}
}

void AClaw::StartPauseForAnimations()
{
	this->PausedForAnimation = true;
}

void AClaw::StopPauseForAnimations()
{
	this->PausedForAnimation = false;
}

void AClaw::Track(float DeltaTime)
{
	if (IsValid(this->CurrentObjective))
	{
		bool isAtEndOfRail; // Out variable for moving crane box
		bool isAtObjective = this->MoveCraneBoxTowardsLocation(this->CurrentObjective->GetActorLocation(), this->CraneBoxSpeed, DeltaTime, isAtEndOfRail);
		if (isAtObjective)
		{
			this->Timer += DeltaTime;
		}
		else
		{
			// Reset time to catch
			this->Timer = 0.0f;
		}
	}
}

bool AClaw::MoveCraneBoxTowardsLocation(FVector Location, float Speed, float DeltaTime, bool& isAtEndOfRail)
{
	bool isAtLocation = false;
	isAtEndOfRail = false;

	float DistanceToLocation = Location.X - this->CraneBoxComponent->GetComponentLocation().X;
	if (abs(DistanceToLocation) < CloseEnough)
	{
		// Objective in catching range
		isAtLocation = true;
		this->StopOnClawMoveFX();
	}
	else
	{
		this->PlayOnClawMoveFX();
	}

	// Move to objective
	if (abs(DistanceToLocation) > 1.0f) // Don't freak out if we're over objective
	{
		// Don't overshoot movement and move past the destination
		float MoveOffset = Speed * DeltaTime;
		if (abs(DistanceToLocation) < MoveOffset)
		{
			MoveOffset = abs(DistanceToLocation);
		}

		FVector NewLocation = this->CraneBoxComponent->GetComponentLocation();
		if (DistanceToLocation > 0 && HasBlock(this->CheckRightHitBox, nullptr) && (NewLocation.X + MoveOffset) < this->RightLimit)
		{
			// Objective is to our right
			NewLocation.X += MoveOffset;
		}
		else if (DistanceToLocation < 0 && HasBlock(this->CheckLeftHitBox, nullptr) && (NewLocation.X - MoveOffset) > this->LeftLimit)
		{
			NewLocation.X -= MoveOffset;
		}
		else
		{
			isAtEndOfRail = true;
		}
		this->CraneBoxComponent->SetWorldLocation(NewLocation, false, nullptr, ETeleportType::TeleportPhysics);

	}

	return isAtLocation;
}

void AClaw::CarryObjective(float DeltaTime)
{
	bool isAtEndOfRail;
	bool isAtObjective = this->MoveCraneBoxTowardsLocation(this->LocationObjective, 0.7 * this->CraneBoxSpeed, DeltaTime, isAtEndOfRail);
	if (isAtObjective || isAtEndOfRail)
	{
		if (this->CaughtCharacter)
		{
			// Move to other end
			this->LocationObjective = (abs(this->CraneBoxComponent->GetComponentLocation().X - this->RightLimit) > abs(this->CraneBoxComponent->GetComponentLocation().X - this->LeftLimit)) ?
				FVector(this->RightLimit, 0.0f, 0.0f) : FVector(this->LeftLimit, 0.0f, 0.0f);
		}
		else
		{
			// Lower block
			FVector End = FVector(this->Mesh->GetComponentLocation().X, this->ClawMesh->GetComponentLocation().Y, this->LowerLimit + 100.0f);
			this->MoveClawComponent->MoveClaw(this->ClawMovementCurve, this->ClawMesh, End, 0.5 * this->ClawLoweringSpeed, false);
			this->Lowering = true;
		}
	}
}


void AClaw::OnCapturedActorStruggle_Implementation(int StruggleAttemptNum, int StrugglesRemainingToEscape, float TimeSinceCaptureStarted)
{
	UE_LOG(LogTemp, Display, TEXT("Character struggling to escape: NumPresses = %d, TimeSinceCaptureStarted = %f"), StruggleAttemptNum, TimeSinceCaptureStarted);
	PlayOnCharacterStruggleFX();
}

void AClaw::OnCapturedActorEscape_Implementation(int StruggleAttemptNum, float TimeSinceCaptureStarted)
{
	UE_LOG(LogTemp, Display, TEXT("Character escaped!"));
	this->CaughtCharacter = false;
	PlayOnReleaseFX();

	if (!this->Lowering && !this->Raising)
	{
		// If we're at top, start tracking again
		StartNewObjective();
	}
}


void AClaw::OnCraneBoxDestroyed(AActor* DestroyedCraneBox)
{
	DestroyAttachment(); // Plays on destroyed fx
}


void AClaw::StartNewObjective()
{
	this->CurrentObjective = nullptr;
	if (this->KaijuInTarget)
	{
		SetCharacterObjective();
	}
	else if (this->MoveBlocks)
	{
		SetNewBlockObjective();
	}
	this->Tracking = true;
}

void AClaw::SetCharacterObjective()
{
	if (IsValid(this->Character))
	{
		this->CurrentObjective = this->Character;
		PlayOnReadyFX();
	}
}

void AClaw::SetNewBlockObjective()
{
	if (this->MovableBlocks.Num() == 0)
	{
		return;
	}

	this->CurrentObjective = this->MovableBlocks[this->BlockIterator % this->MovableBlocks.Num()];
	++this->BlockIterator;
}

void AClaw::FindPlaceToDropBox()
{

}

void AClaw::DropBlock()
{
	if (this->CurrentObjective) // Should be a block
	{
		ABreakableStructure* Block = Cast<ABreakableStructure>(this->CurrentObjective);
		if (IsValid(Block))
		{
			UCaptureAbility::ReleaseActor(this, Block);
			Block->SetIsSimulatingPhysics(true);
			this->CaughtBlock = false;
		}
	}
}

void AClaw::InitActiveLimits()
{
	this->InitialClawHeight = this->ClawMesh->GetComponentLocation().Z;
	this->RightLimit = this->KaijuTargetArea->GetComponentLocation().X + this->KaijuTargetArea->GetScaledBoxExtent().X;
	this->LeftLimit = this->KaijuTargetArea->GetComponentLocation().X - this->KaijuTargetArea->GetScaledBoxExtent().X;
	this->LowerLimit = this->KaijuTargetArea->GetComponentLocation().Z - this->KaijuTargetArea->GetScaledBoxExtent().Z;
}

void AClaw::CheckAttachedBlock()
{
	ABuildingBlock* AttachedBlock = nullptr;
	if (!HasBlock(AttachBox, AttachedBlock))
	{
		// TODO: REMOVE WIDGET PROMPT IF IT EXISTS

		if (this->BB_CraneBox)
		{
			this->BB_CraneBox->DestroyStructure();
		}
		this->DestroyClaw(); // Redundant
	}
}

bool AClaw::HasBlock(UPrimitiveComponent* Component, ABuildingBlock* Block)
{
	TSet<AActor*> OverlappedActors;

	Component->GetOverlappingActors(OverlappedActors, ABuildingBlock::StaticClass());

	for (AActor* Actor : OverlappedActors)
	{
		Block = Cast<ABuildingBlock>(Actor);
		if (IsValid(Block))
		{
			return true;
		}
	}

	return false;
}

