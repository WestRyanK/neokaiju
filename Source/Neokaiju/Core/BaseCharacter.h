// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "BaseCharacter.generated.h"

class UBaseGameplayAbility;

// A macro which makes it easy to expose a getter for a property in the character's AttributeSet. See KaijuCharacter for an example.
#define ATTRIBUTESET_PROPERTY_GETTER(PropertyName) \
	UFUNCTION(BlueprintGetter) \
	float Get##PropertyName##() const \
	{ \
		return this->AttributeSet->Get##PropertyName##(); \
	} 

/**
*  Every character in our game should inherit from this. It provides access to the AbilitySystem and automatically grants abilities to the character.
*/
UCLASS()
class NEOKAIJU_API ABaseCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		class UAbilitySystemComponent* AbilitySystemComponent;

public:
	// Sets default values for this character's properties
	ABaseCharacter();

	// Inherited via IAbilitySystemInterface
	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	bool bShouldApplyInitialEffects = true;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called After constructor but before BeginPlay().
	//virtual void PostInitProperties();


	// Property Specifiers: Explains BlueprintReadOnly and EditAnywhere
	// https://docs.unrealengine.com/en-US/ProgrammingAndScripting/GameplayArchitecture/Properties/index.html

	// Abilities that are added to this array in the BP will automatically
	// be granted to the character when it is created.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
		TArray<TSubclassOf<class UBaseGameplayAbility>> CharacterStandardAbilities;


	// Sets the Character's Level and updates their attributes to reflect the values in CharacterAttributesLevelValues.
	//void SetCharacterLevel(int32 CharacterLevel);

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
		TArray<TSubclassOf<class UGameplayEffect>> CharacterInitialEffects;

private:
	void SetCharacterInitialEffects();

	TArray<FGameplayAbilitySpecHandle> CharacterStandardAbilitySpecHandles;

	// Must be called from BeginPlay to give our character all of his abilities.
	void GrantCharacterStandardAbilities(int32 CharacterLevel = 1);

	// Updates the Standard Abilities of the Character to use new values based on the current CharacterLevel
	// without affecting other abilities.
	void UpdateCharacterStandardAbilitiesWithLevel(int32 CharacterLevel);

};
