// Fill out your copyright notice in the Description page of Project Settings.


#include "SpikeBlock.h"
#include "Neokaiju/KaijuCharacter/KaijuCharacter.h"
#include "Engine.h"


ASpikeBlock::ASpikeBlock()
{
	this->SpikeOverlapBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SpikeOverlapBox"));
	this->SpikeOverlapBox->SetupAttachment(this->Mesh);
	this->SpikeOverlapBox->SetRelativeScale3D(FVector(1.05f, 1.05f, 1.05f));
	FVector min = FVector(0,0,0);
	FVector max = FVector(0,0,0);
	MaxLevelKaijuToEffect = 3.0f;
	this->Mesh->GetLocalBounds(min, max);
	FVector center = (max + min) / 2.0;
	FVector extent = max - center;
	this->SpikeOverlapBox->SetBoxExtent(extent);
	this->SpikeOverlapBox->SetRelativeLocation(center);
}

void ASpikeBlock::BeginPlay()
{
	Super::BeginPlay();

	this->Mesh->OnComponentHit.AddUniqueDynamic(this, &ASpikeBlock::OnHit);
	
}

void ASpikeBlock::OnHit(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	FVector NormalImpulse,
	const FHitResult& Hit)
{
	AKaijuCharacter* Character = Cast<AKaijuCharacter>(OtherActor);
	if (IsValid(Character))
	{

		if (Character->GetKaijuLevel() <= this->MaxLevelKaijuToEffect) {
			if (IsValid(this->SpikeDamageEffect))
			{
				AbilitySystemComponent->ApplyGameplayEffectToTarget(
					this->SpikeDamageEffect.GetDefaultObject(),
					Character->GetAbilitySystemComponent(),
					1);

				UE_LOG(LogTemp, Display, TEXT("Spiked Player, Health at: %f"), Character->GetHealth());
			}
			GetWorldTimerManager().SetTimer(timerHandle, this, &ASpikeBlock::CheckForOverlaps, 0.1f, true, 0.1f);
		}
		else {
			this->DestroyStructure();
			this->Destroy();
		}

	}
}

void ASpikeBlock::CheckForOverlaps() {
	TSet<AActor*> OverlappingActors;
	this->SpikeOverlapBox->GetOverlappingActors(OverlappingActors);
	OverlappingActors.Remove(this);

	bool containsCharacter = false;

	for (AActor* OverlappingActor : OverlappingActors) {
		AKaijuCharacter* Character = Cast<AKaijuCharacter>(OverlappingActor);
		if (IsValid(Character))
		{
			
			if (Character->GetKaijuLevel() <= this->MaxLevelKaijuToEffect) {
				UE_LOG(LogTemp, Display, TEXT("Kaiju Level: %f"), Character->GetKaijuLevel());
				
				if (IsValid(this->SpikeDamageEffect))
				{
					AbilitySystemComponent->ApplyGameplayEffectToTarget(
						this->SpikeDamageEffect.GetDefaultObject(), 
						Character->GetAbilitySystemComponent(), 
						1);

					UE_LOG(LogTemp, Display, TEXT("Spiked Player, Health at: %f"), Character->GetHealth());
				}
				containsCharacter = true;
			}
			else {
				GetWorldTimerManager().ClearTimer(timerHandle);
				this->DestroyStructure();
				this->Destroy();
			}
		}
	}

	if (!containsCharacter) {
		GetWorldTimerManager().ClearTimer(timerHandle);
	}
}