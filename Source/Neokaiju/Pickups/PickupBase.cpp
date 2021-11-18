// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupBase.h"
#include "Components/StaticMeshComponent.h"
#include "Neokaiju/Pickups/PickupSubsystem.h"
#include "Neokaiju/KaijuCharacter/KaijuCharacter.h"

// Sets default values
APickupBase::APickupBase()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RootMesh"));
	RootComponent = Mesh;
	Mesh->SetSimulatePhysics(false);
	Mesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	Mesh->BodyInstance.bLockYTranslation = true;
	Mesh->BodyInstance.bLockXRotation = true;
	Mesh->BodyInstance.bLockZRotation = true;

	Mesh->OnComponentBeginOverlap.AddDynamic(this, &APickupBase::BeginOverlap);
	Mesh->OnComponentHit.AddDynamic(this, &APickupBase::OnHit);
}

// Called when the game starts or when spawned
void APickupBase::BeginPlay()
{
	Super::BeginPlay();

}

void APickupBase::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	AKaijuCharacter* Character = Cast<AKaijuCharacter>(OtherActor);

	if (IsValid(Character) && OtherComp == Character->GetRootComponent())
	{
		PickUp(Character);
	}
}

void APickupBase::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult& SweepResult)
{
	AKaijuCharacter* Character = Cast<AKaijuCharacter>(OtherActor);

	if (IsValid(Character) && OtherComp == Character->GetRootComponent())
	{
		PickUp(Character);
	}
}

void APickupBase::PickUp(AKaijuCharacter* Character)
{
	if (!this->bWasPickedUp)
	{
		this->bWasPickedUp = true;
		if (IsValid(EffectToApply))
		{
			Character->GetAbilitySystemComponent()->ApplyGameplayEffectToSelf(
				EffectToApply.GetDefaultObject(), 1,
				Character->GetAbilitySystemComponent()->MakeEffectContext());
		}

		this->OnPickedUp(Character);
		UPickupSubsystem* Subsystem = this->GetWorld()->GetSubsystem<UPickupSubsystem>();
		Subsystem->PickupPickedUp(this);
		this->Destroy();
	}
}
