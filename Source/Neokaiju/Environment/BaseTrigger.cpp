// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseTrigger.h"
#include "Components/BoxComponent.h"
#include "TimerManager.h"
#include "Engine/World.h"


ABaseTrigger::ABaseTrigger()
{
	this->PrimaryActorTick.bCanEverTick = true;
	this->GetCollisionComponent()->OnComponentBeginOverlap.AddUniqueDynamic(this, &ABaseTrigger::OnBeginOverlap);
	this->GetCollisionComponent()->OnComponentEndOverlap.AddUniqueDynamic(this, &ABaseTrigger::OnEndOverlap);
}

void ABaseTrigger::BeginPlay()
{
	Super::BeginPlay();

	this->GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ABaseTrigger::InitialOverlapCheck);
}

void ABaseTrigger::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void ABaseTrigger::InitialOverlapCheck()
{
	TSet<AActor*> OverlappingActors;
	this->GetCollisionComponent()->GetOverlappingActors(OverlappingActors, this->TriggerForActorOfType);

	FHitResult SweepResult;
	for (AActor* OverlappingActor : OverlappingActors)
	{
		UPrimitiveComponent* OverlappingRoot = Cast<UPrimitiveComponent>(OverlappingActor->GetRootComponent());
		if (IsValid(OverlappingRoot))
		{
			this->OnBeginOverlap(this->GetCollisionComponent(), OverlappingActor, OverlappingRoot, 0, false, SweepResult);
		}
	}
}


void ABaseTrigger::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (this->IsCorrectActorAndComponent(OtherActor, OtherComp) && !this->bIsInsideTrigger)
	{
		this->bIsEnterTriggerPending = true;
		this->bIsExitTriggerPending = false;
		this->bIsInsideTrigger = true;
		this->TriggerActor = OtherActor;

		UE_LOG(LogTemp, Display, TEXT("Enter Trigger Pending"));
	}
}

void ABaseTrigger::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (this->IsCorrectActorAndComponent(OtherActor, OtherComp) && this->bIsInsideTrigger)
	{
		this->bIsExitTriggerPending = true;
		this->bIsEnterTriggerPending = false;
		this->bIsInsideTrigger = false;
		UE_LOG(LogTemp, Display, TEXT("Exit Trigger Pending"));
	}
}

bool ABaseTrigger::IsCorrectActorAndComponent(AActor* Actor, UPrimitiveComponent* Component)
{
	bool bIsCorrectActor = Actor->GetClass()->IsChildOf(this->TriggerForActorOfType);
	bool bIsCorrectComponent = (Actor->GetRootComponent() == Component);
	return bIsCorrectActor && bIsCorrectComponent;
}

void ABaseTrigger::FinishTransition()
{
	this->bIsTransitioning = false;
	UE_LOG(LogTemp, Display, TEXT("Transition Ended"));
}

bool ABaseTrigger::ShouldPendingTriggerExecute(bool bIsTriggerPending)
{
	bool bCanTrigger_MultipleTimes = this->bCanBeTriggeredMultipleTimes || !this->bHasBeenTriggered;
	bool bCanTrigger_Interrupting = this->bCanInterruptTransitions || !this->bIsTransitioning;

	return bIsTriggerPending && bCanTrigger_Interrupting && bCanTrigger_MultipleTimes;
}

void ABaseTrigger::Tick(float DeltaTime)
{
	if (this->ShouldPendingTriggerExecute(this->bIsEnterTriggerPending))
	{
		UE_LOG(LogTemp, Display, TEXT("Enter Trigger Started"));
		this->bIsTransitioning = true;
		this->bIsEnterTriggerPending = false;
		this->OnBeginTrigger_BP(this->TriggerActor);
	}

	if (this->ShouldPendingTriggerExecute(this->bIsExitTriggerPending))
	{
		UE_LOG(LogTemp, Display, TEXT("Exit Trigger Started"));
		this->bIsTransitioning = true;
		this->bIsExitTriggerPending = false;
		this->OnEndTrigger_BP(this->TriggerActor);
		this->bHasBeenTriggered = true;
		this->TriggerActor = nullptr;
	}
}

AActor* ABaseTrigger::GetTriggerActor() const
{
	return this->TriggerActor;
}

bool ABaseTrigger::GetIsTriggerActorInside() const
{
	return this->bIsInsideTrigger;
}

bool ABaseTrigger::GetIsTriggerEnabled() const
{
	return this->bIsTriggerEnabled;
}

void ABaseTrigger::SetIsTriggerEnabled(bool bIsTriggerEnabledValue)
{
	if (this->bIsTriggerEnabled != bIsTriggerEnabledValue)
	{
		UE_LOG(LogTemp, Display, TEXT("From %d to %d"), this->bIsTriggerEnabled, bIsTriggerEnabledValue);
		this->bIsTriggerEnabled = bIsTriggerEnabledValue;
		ECollisionEnabled::Type CollisionValue;
		if (this->bIsTriggerEnabled)
		{
			CollisionValue = ECollisionEnabled::Type::QueryOnly;
		}
		else
		{
			CollisionValue = ECollisionEnabled::Type::NoCollision;
		}
		this->GetCollisionComponent()->SetCollisionEnabled(CollisionValue);
	}

}

void ABaseTrigger::OnBeginTrigger_BP_Implementation(AActor* TriggeringActor)
{
	this->OnBeginTrigger(TriggeringActor);
}

void ABaseTrigger::OnEndTrigger_BP_Implementation(AActor* TriggeringActor)
{
	this->OnEndTrigger(TriggeringActor);
}
