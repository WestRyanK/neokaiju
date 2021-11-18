// Fill out your copyright notice in the Description page of Project Settings.


#include "Spikes.h"
#include "TimerManager.h"

ASpikes::ASpikes()
{
	this->SpikeHitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SpikeHitBox"));
	this->SpikeBlockBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SpikeBlockBox"));
	this->SpikeHitBox->SetupAttachment(this->RootComponent);
	this->SpikeBlockBox->SetupAttachment(this->SpikeHitBox);
	this->SpikeBlockBox->SetRelativeScale3D(FVector(0.95f, 0.95f, 0.95f));
	
}

void ASpikes::BeginPlay()
{
	Super::BeginPlay();

	this->SpikeHitBox->OnComponentBeginOverlap.AddUniqueDynamic(this, &ASpikes::OnTouch);
	
}



void ASpikes::OnTouch(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// hurt the player

	AKaijuCharacter* Character = Cast<AKaijuCharacter>(OtherActor);
	if (IsValid(Character))
	{
		if (Character->GetKaijuLevel() < this->AttributeSet->GetMaximumKaijuLevelToEffect()) {
			UE_LOG(LogTemp, Display, TEXT("Kaiju Level: %f"), Character->GetKaijuLevel());
			UE_LOG(LogTemp, Display, TEXT("Max: %f"), this->AttributeSet->GetMaximumKaijuLevelToEffect());
			if (IsValid(this->SpikeDamageEffect))
			{
				AbilitySystemComponent->ApplyGameplayEffectToTarget(this->SpikeDamageEffect.GetDefaultObject(), Character->GetAbilitySystemComponent(), 1);

				UE_LOG(LogTemp, Display, TEXT("Spiked Player, Health at: %f"), Character->GetHealth());
			}
			GetWorldTimerManager().SetTimer(timerHandle, this, &ASpikes::CheckForOverlaps, 0.1f, true, 0.1f);
		}
		else
		{
			this->DestroyAttachment();
		}
	}
}

void ASpikes::CheckForOverlaps() {
	TSet<AActor*> OverlappingActors;
	this->SpikeHitBox->GetOverlappingActors(OverlappingActors);
	OverlappingActors.Remove(this);

	if (OverlappingActors.Num() <= 0) {
		GetWorldTimerManager().ClearTimer(timerHandle);
	}

	for (AActor* OverlappingActor : OverlappingActors) {
		AKaijuCharacter* Character = Cast<AKaijuCharacter>(OverlappingActor);
		if (IsValid(Character))
		{
			if (Character->GetKaijuLevel() < this->AttributeSet->GetMaximumKaijuLevelToEffect()) {
				UE_LOG(LogTemp, Display, TEXT("Kaiju Level: %f"), Character->GetKaijuLevel());
				UE_LOG(LogTemp, Display, TEXT("Max: %f"), this->AttributeSet->GetMaximumKaijuLevelToEffect());
				if (IsValid(this->SpikeDamageEffect))
				{
					AbilitySystemComponent->ApplyGameplayEffectToTarget(this->SpikeDamageEffect.GetDefaultObject(), Character->GetAbilitySystemComponent(), 1);

					UE_LOG(LogTemp, Display, TEXT("Spiked Player, Health at: %f"), Character->GetHealth());
				}
			}
			else
			{
				this->DestroyAttachment();
			}
		}
	}
}

