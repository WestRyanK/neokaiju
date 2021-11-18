// Fill out your copyright notice in the Description page of Project Settings.


#include "MoveAlongCurveAbilityTask.h"
#include "Curves/CurveFloat.h"
#include "GameFramework/Character.h"

UMoveAlongCurveAbilityTask::UMoveAlongCurveAbilityTask()
{
	this->bTickingTask = true;
	this->bIsMovementAbsoluteAlongCurve = true;
}

void UMoveAlongCurveAbilityTask::Activate()
{
	this->StartTime = this->GetWorld()->GetTimeSeconds();
	this->EndTime = this->StartTime + this->MoveAlongCurveForDuration;

	this->Character = Cast<ACharacter>(this->GetAvatarActor());
	if (IsValid(this->Character))
	{
		this->LocationAtLastDirectionChange = this->Character->GetActorLocation();
		this->Character->OnActorHit.AddUniqueDynamic(this, &UMoveAlongCurveAbilityTask::OnHit);
	}
}

void UMoveAlongCurveAbilityTask::Finish()
{
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		this->OnCompleted.Broadcast();
	}
	this->EndTask();
}

void UMoveAlongCurveAbilityTask::OnHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{

}

void UMoveAlongCurveAbilityTask::OnDestroy(bool bInOwnerFinished)
{
	if (IsValid(this->Character))
	{
		this->Character->OnActorHit.RemoveDynamic(this, &UMoveAlongCurveAbilityTask::OnHit);
	}
	Super::OnDestroy(bInOwnerFinished);
}

void UMoveAlongCurveAbilityTask::TickTask(float DeltaTime)
{
	if (this->GetWorld()->GetTimeSeconds() >= this->EndTime)
	{
		this->Finish();
	}

	if (IsValid(this->Character))
	{
		float PercentComplete = this->GetPercentComplete();
		float Progress = this->GetProgressAtPercent(PercentComplete);
		float ProgressSinceLastDirectionChange = Progress - this->ProgressAtLastDirectionChange;

		if (this->bIsMovementAbsoluteAlongCurve)
		{
			FVector NewLocation = this->LocationAtLastDirectionChange + ProgressSinceLastDirectionChange * this->MoveAlongCurveInDirection * this->MoveAlongCurveOfLength;
			this->Character->SetActorLocation(NewLocation, true);
		}
		else
		{
			float DerivativeAtPercent = this->GetDerivativeAtPercent(PercentComplete, DeltaTime / this->MoveAlongCurveForDuration);
			float Offset = DerivativeAtPercent * this->MoveAlongCurveOfLength * DeltaTime / this->MoveAlongCurveForDuration;
			this->Character->AddActorWorldOffset(Offset * this->MoveAlongCurveInDirection, true);
			ABTASK_LOG(TEXT("Percent %f, Progress %f, Derivative %f, Delta %f, Offset %f"), PercentComplete, Progress, DerivativeAtPercent, DeltaTime / this->MoveAlongCurveForDuration, Offset);
		}
	}

	if (ShouldBroadcastAbilityTaskDelegates())
	{
		this->OnTick.Broadcast(DeltaTime, this->MoveAlongCurveInDirection);
	}
}

float UMoveAlongCurveAbilityTask::GetDerivativeAtPercent(float PercentComplete, float Delta) const
{
	float Derivative = (this->GetProgressAtPercent(PercentComplete) - this->GetProgressAtPercent(PercentComplete - Delta)) / Delta;
	return Derivative;
}

float UMoveAlongCurveAbilityTask::GetProgressAtPercent(float PercentComplete) const
{
	if (IsValid(this->PositionOverTime))
	{
		float Progress = this->PositionOverTime->GetFloatValue(PercentComplete);
		return Progress;
	}
	else
	{
		return PercentComplete;
	}
}

float UMoveAlongCurveAbilityTask::GetPercentComplete()
{
	float PercentComplete = (this->GetWorld()->GetTimeSeconds() - this->StartTime) / this->MoveAlongCurveForDuration;
	return PercentComplete;
}

