// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseJumpAbility.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SceneComponent.h"
#include "Neokaiju/KaijuCharacter/KaijuCharacter.h"


UBaseJumpAbility::UBaseJumpAbility()
{
	//this->InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerExecution;
	this->bRetriggerInstancedAbility = true;
}

void UBaseJumpAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!this->CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		this->EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AB_LOG(TEXT("Air Jump: #%d"), this->ConsecutiveAbilitiesCount);

	this->Character = Cast<AKaijuCharacter>(ActorInfo->AvatarActor.Get());
	if (IsValid(this->Character))
	{
		float JumpHeight = this->Character->GetJumpHeight() * this->JumpHeightMultiplier;

		FVector JumpDirection = FVector::UpVector;
		this->OnJump(this->Character->GetMesh(), this->FeetCenterSocketName, this->ConsecutiveAbilitiesCount);
		this->ExecuteJump(JumpDirection, JumpHeight);
	}
	else
	{
		AB_LOG(TEXT("No valid KaijuCharacter for JumpAbility"));
		this->EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
	}
}

void UBaseJumpAbility::ExecuteJump(FVector JumpDirection, float JumpHeight)
{
}

float UBaseJumpAbility::CalculateVelocityForJumpHeight(float JumpHeight, float GravityScale)
{
	float Gravity;
	if (GravityScale == 0.0f)
	{
		Gravity = this->Character->GetCharacterMovement()->GetGravityZ();
	}
	else
	{
		float BaseGravity = this->Character->GetCharacterMovement()->UMovementComponent::GetGravityZ();
		Gravity = BaseGravity * GravityScale;
	}
	
	float JumpVelocity = FMath::Sqrt(JumpHeight * (-2.0f * Gravity)); // How fast do I need to jump with current gravity in order to reach JumpHeight?
	return JumpVelocity;
}
