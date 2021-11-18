// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"
#include "GameplayAbilitySpec.h"
#include "Neokaiju/Ability/Core/Abilities/BaseGameplayAbility.h"
#include "AbilitySystemComponent.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	this->AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	this->GrantCharacterStandardAbilities();
	this->SetCharacterInitialEffects();
}

void ABaseCharacter::SetCharacterInitialEffects()
{
	if (this->bShouldApplyInitialEffects)
	{
		for (const TSubclassOf<UGameplayEffect> InitialEffect : this->CharacterInitialEffects)
		{
			this->AbilitySystemComponent->ApplyGameplayEffectToSelf(InitialEffect.GetDefaultObject(), 1, this->AbilitySystemComponent->MakeEffectContext());
		}
	}
}

void ABaseCharacter::GrantCharacterStandardAbilities(int32 CharacterLevel)
{
	// If this is called a second time, we don't want abilities doubled up, so clear any previously added standard abilities.
	for (const FGameplayAbilitySpecHandle& Handle : this->CharacterStandardAbilitySpecHandles)
	{
		this->AbilitySystemComponent->ClearAbility(Handle);
	}
	this->CharacterStandardAbilitySpecHandles.Empty();

	// Add standard abilities to ASC.
	for (TSubclassOf<class UBaseGameplayAbility> Ability : this->CharacterStandardAbilities)
	{
		if (IsValid(Ability))
		{
			// TODO: Do we want any abilities directly tied to an input?
			int32 AbilityInputID = static_cast<int32>(Ability.GetDefaultObject()->AbilityInputID);
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Ability, CharacterLevel, AbilityInputID, this);
			FGameplayAbilitySpecHandle Handle = this->AbilitySystemComponent->GiveAbility(AbilitySpec);
			this->CharacterStandardAbilitySpecHandles.Add(Handle);
		}
	}
}

void ABaseCharacter::UpdateCharacterStandardAbilitiesWithLevel(int32 CharacterLevel)
{
	for (const FGameplayAbilitySpecHandle& Handle : this->CharacterStandardAbilitySpecHandles)
	{
		FGameplayAbilitySpec* AbilitySpec = this->AbilitySystemComponent->FindAbilitySpecFromHandle(Handle);
		if (AbilitySpec)
		{
			AbilitySpec->Level = CharacterLevel;
		}
	}
}

UAbilitySystemComponent* ABaseCharacter::GetAbilitySystemComponent() const
{
	return this->AbilitySystemComponent;
}

