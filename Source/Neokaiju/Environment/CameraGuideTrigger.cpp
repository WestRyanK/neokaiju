// Fill out your copyright notice in the Description page of Project Settings.


#include "CameraGuideTrigger.h"
#include "Components/SplineComponent.h"
#include "Components/SceneComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

ACameraGuideTrigger::ACameraGuideTrigger()
{
	this->CameraPath = CreateDefaultSubobject<USplineComponent>("CameraPath");
}

void ACameraGuideTrigger::OnBeginTrigger(AActor* TriggeringActor)
{
	if (IsValid(TriggeringActor))
	{
		TArray<UActorComponent*> TaggedComponents = TriggeringActor->GetComponentsByTag(USceneComponent::StaticClass(), FName(*this->TagOfComponentToGuide));
		if (TaggedComponents.Num() > 0)
		{
			this->GuidedActorComponent = Cast<USceneComponent>(TaggedComponents[0]);
			this->OriginalGuidedActorComponentLocation = FVector(0, 0, 0);// this->GuidedActorComponent->GetRelativeLocation();
		}
	}
	this->bIsTransitioningIn = true;
}

void ACameraGuideTrigger::OnEndTrigger(AActor* TriggeringActor)
{
	this->bIsTransitioningOut = true;
}

void ACameraGuideTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (IsValid(this->GuidedActorComponent))
	{
		if (this->bIsTransitioningIn)
		{
			FVector GuideLocation = this->GetGuideLocation();
			FVector InterpolatedLocation = FMath::VInterpConstantTo(this->GuidedActorComponent->GetComponentLocation(), GuideLocation, DeltaTime, this->InterpSpeed);
			this->GuidedActorComponent->SetWorldLocation(InterpolatedLocation, true);

			float DistanceFromDestination = FVector::Dist(GuideLocation, InterpolatedLocation);
			//UE_LOG(LogTemp, Display, TEXT("Distance %f"), DistanceFromDestination);
			if (DistanceFromDestination < DELTA)
			{
				this->bIsTransitioningIn = false;
				this->FinishTransition();
			}
		}
		else if (this->bIsTransitioningOut)
		{
			FVector GuidedActorRelativeLocation = this->GuidedActorComponent->GetRelativeLocation();
			FVector InterpolatedLocation = FMath::VInterpConstantTo(GuidedActorRelativeLocation, this->OriginalGuidedActorComponentLocation, DeltaTime, this->InterpSpeed);
			this->GuidedActorComponent->SetRelativeLocation(InterpolatedLocation, true);

			float DistanceFromDestination = FVector::Dist(GuidedActorRelativeLocation, this->OriginalGuidedActorComponentLocation);
			//UE_LOG(LogTemp, Display, TEXT("Distance %f"), DistanceFromDestination);
			if (DistanceFromDestination < DELTA)
			{
				this->GuidedActorComponent->SetRelativeLocation(this->OriginalGuidedActorComponentLocation);
				this->bIsTransitioningOut = false;
				this->FinishTransition();
			}
		}
		else if (this->GetIsTriggerActorInside())
		{
			FVector GuideLocation = this->GetGuideLocation();
			this->GuidedActorComponent->SetWorldLocation(GuideLocation, true);
		}
	}
}

FVector ACameraGuideTrigger::GetGuideLocation()
{
	FVector TargetLocation = this->GuidedActorComponent->GetComponentLocation();
	AActor* TriggeringActor = this->GetTriggerActor();
	TargetLocation.X = TriggeringActor->GetActorLocation().X;
	TargetLocation.Z = TriggeringActor->GetActorLocation().Z;

	FVector GuideLocation = this->CameraPath->FindLocationClosestToWorldLocation(TargetLocation, ESplineCoordinateSpace::World);
	if (this->bIgnoreYAxis)
	{
		GuideLocation.Y = TargetLocation.Y;
	}

	return GuideLocation;
}
