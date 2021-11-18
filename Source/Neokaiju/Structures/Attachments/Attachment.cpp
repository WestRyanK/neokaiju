// Fill out your copyright notice in the Description page of Project Settings.


#include "Attachment.h"
#include "Components/BoxComponent.h"

// Sets default values
AAttachment::AAttachment()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
	Mesh->SetRelativeLocation(FVector(0.0, 0.0, 50.0), false, nullptr, ETeleportType::TeleportPhysics);

	ScaleComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scale"));
	ScaleComponent->SetupAttachment(RootComponent);
	ScaleComponent->SetRelativeLocation(FVector(0.0, 0.0, 50.0), false, nullptr, ETeleportType::TeleportPhysics);

	AttachBox = CreateDefaultSubobject<UBoxComponent>(TEXT("AttachBox"));
	AttachBox->SetupAttachment(ScaleComponent);
	AttachBox->SetRelativeLocation(FVector(0.0, 0.0, -55.0), false, nullptr, ETeleportType::TeleportPhysics);
	AttachBox->SetBoxExtent(FVector(25.0, 25.0, 3.0), true);

	AttributeSet = CreateDefaultSubobject<UAttachmentAttributeSet>(TEXT("AttributeSet"));
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>("AbilitySystemComponent");

}

// Called when the game starts or when spawned
void AAttachment::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(this->InitialAttributeValues))
	{
		this->AbilitySystemComponent->ApplyGameplayEffectToSelf(this->InitialAttributeValues.GetDefaultObject(), 1, this->AbilitySystemComponent->MakeEffectContext());
	}

	this->UpdateOverlaps();
	InitAttachBlock();

	// Set the events for when the attached block is destroyed or becomes unstable
	if (IsValid(BlockAttached))
	{
		BlockAttached->OnDestroyed.AddDynamic(this, &AAttachment::AttachedBlockDestroyed);
		BlockAttached->OnStructureUnstable.AddDynamic(this, &AAttachment::AttachedBlockUnstable);
	}
}

void AAttachment::OnInteract()
{
}

// Called every frame
void AAttachment::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAttachment::DestroyAttachment()
{
	PlayOnDestroyedFX();

	this->Destroy();
}


UAbilitySystemComponent* AAttachment::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AAttachment::InitAttachBlock()
{
	TSet<AActor*> OverlappingActors;

	AttachBox->GetOverlappingActors(OverlappingActors, ABreakableStructure::StaticClass());

	UE_LOG(LogTemp, Display, TEXT("%s Attached to: "), *this->GetName());

	for (AActor* Actor : OverlappingActors)
	{
		UE_LOG(LogTemp, Display, TEXT("loop"));
		if (IsValid(Actor))
		{
			SetAttachedIfBreakableStructure(Actor);
			UE_LOG(LogTemp, Display, TEXT("%s Attached to: "), *this->GetName());
			UE_LOG(LogTemp, Display, TEXT("%s"), *BlockAttached->GetName());
		}

	}



}

void AAttachment::SetAttachedIfBreakableStructure(AActor* Actor)
{
	ABreakableStructure* Attached = Cast<ABreakableStructure>(Actor);
	if (IsValid(Attached))
	{

		BlockAttached = Attached;

	}
}

// Event when the attached block is unstable. Default functionality is that the attachment is destroyed
void AAttachment::AttachedBlockUnstable(ABreakableStructure* UnstableStructure)
{
	ABreakableStructure* UnstableBlock = Cast<ABreakableStructure>(UnstableStructure);
	if (IsValid(UnstableBlock))
	{
		Destroy();
	}
}

// Event when the attached block is destroyed. Default functionality is that the attachment is destroyed
void AAttachment::AttachedBlockDestroyed(AActor* DestroyedActor)
{
	ABreakableStructure* DestroyedBlock = Cast<ABreakableStructure>(DestroyedActor);
	if (IsValid(DestroyedBlock))
	{
		Destroy();
	}

}