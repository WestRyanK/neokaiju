// Fill out your copyright notice in the Description page of Project Settings.


#include "JumpAlongCurveAbilityTask.h"

UJumpAlongCurveAbilityTask* UJumpAlongCurveAbilityTask::JumpAlongCurve(
	UGameplayAbility* OwningAbility,
	FVector JumpDirection,
	float JumpDistance,
	float JumpDuration,
	UCurveFloat* JumpPositionOverTime)
{
	UJumpAlongCurveAbilityTask* JumpAlongCurveAbilityTask = NewAbilityTask<UJumpAlongCurveAbilityTask>(OwningAbility);
	JumpAlongCurveAbilityTask->MoveAlongCurveInDirection = JumpDirection;
	JumpAlongCurveAbilityTask->MoveAlongCurveOfLength = JumpDistance;
	JumpAlongCurveAbilityTask->MoveAlongCurveForDuration = JumpDuration;
	JumpAlongCurveAbilityTask->PositionOverTime = JumpPositionOverTime;
	JumpAlongCurveAbilityTask->bIsMovementAbsoluteAlongCurve = false;

	return JumpAlongCurveAbilityTask;
}

void UJumpAlongCurveAbilityTask::OnHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
	this->Finish();
}
