// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStateTaggerComponent.h"

// Sets default values for this component's properties
UCharacterStateTaggerComponent::UCharacterStateTaggerComponent()
{
	//PrimaryComponentTick.bCanEverTick = true;

	//PrimaryComponentTick.TickInterval = this->UpdateTagTick;
}


// Called when the game starts
void UCharacterStateTaggerComponent::BeginPlay()
{
	Super::BeginPlay();

	this->OwnerCharacter = Cast<ACharacter>(this->GetOwner());
	IAbilitySystemInterface* IASC = Cast<IAbilitySystemInterface>(this->GetOwner());
	if (IASC)
	{
		this->OwnerASC = IASC->GetAbilitySystemComponent();
	}

	this->OwnerCharacter->MovementModeChangedDelegate.AddUniqueDynamic(this, &UCharacterStateTaggerComponent::OnMovementModeChanged);
	this->OnMovementModeChanged(this->OwnerCharacter, this->OwnerCharacter->GetCharacterMovement()->MovementMode, 0);
}

void UCharacterStateTaggerComponent::DestroyComponent(bool bPromoteChildren)
{
	if (IsValid(this->OwnerCharacter))
	{
		this->OwnerCharacter->MovementModeChangedDelegate.RemoveDynamic(this, &UCharacterStateTaggerComponent::OnMovementModeChanged);
	}
	Super::DestroyComponent(bPromoteChildren);
}

//void UCharacterStateTaggerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
//{
//	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
//	this->OnMovementModeChanged(this->OwnerCharacter, this->OwnerCharacter->GetCharacterMovement()->MovementMode, 0);
//}

void UCharacterStateTaggerComponent::OnMovementModeChanged(ACharacter* Character, EMovementMode PrevMovementMode, uint8 PreviousCustomMode)
{
	if (IsValid(this->OwnerASC))
	{
		EMovementMode CurrentMovementMode = this->OwnerCharacter->GetCharacterMovement()->MovementMode;
		TArray<FActiveGameplayEffectHandle> EffectsToCancel;
		this->MovementModeEffectHandles.MultiFind(PrevMovementMode, EffectsToCancel, false);
		for (FActiveGameplayEffectHandle EffectToCancel : EffectsToCancel)
		{
			if (EffectToCancel.IsValid())
			{
				this->OwnerASC->RemoveActiveGameplayEffect(EffectToCancel, -1);
			}
		}
		this->MovementModeEffectHandles.Remove(PrevMovementMode);


		TSubclassOf<UGameplayEffect>* EffectToApply = this->MovementModeEffects.Find(CurrentMovementMode);
		if (this->MovementModeEffects.Contains(CurrentMovementMode) && *EffectToApply)
		{
			FActiveGameplayEffectHandle AppliedEffectHandle = this->OwnerASC->ApplyGameplayEffectToSelf(EffectToApply->GetDefaultObject(), 1, this->OwnerASC->MakeEffectContext());
			this->MovementModeEffectHandles.Add(CurrentMovementMode, AppliedEffectHandle);
		}
		UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EMovementMode"), true);
		//UE_LOG(LogTemp, Display, TEXT("Prev Mode: %s New Mode: %s"), *EnumPtr->GetDisplayNameTextByIndex(PrevMovementMode).ToString(), *EnumPtr->GetDisplayNameTextByIndex(CurrentMovementMode).ToString());
	}
}

