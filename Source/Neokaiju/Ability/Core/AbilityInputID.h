// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"

// Abilities can be tied to a specific button, so that if the ability is granted
// to your character, you can activate it by pressing that button.
// To do that, you must open Project Settings > Input and add the input mapping like you
// normally would. Then you come to this Enum and add a value with the same name as the
// input mapping. Once you compile the project, you can open the ability's blueprint and 
// change the AbilityInputID to match the enum value.
// https://github.com/tranek/GASDocumentation#462-binding-input-to-the-asc
UENUM(BlueprintType)
enum class EAbilityInputID : uint8
{
	None			UMETA(DisplayName = "None"),

	Confirm			UMETA(DisplayName = "Confirm"),

	Cancel			UMETA(DisplayName = "Cancel"),

	Dash			UMETA(DisplayName = "Dash"),

	Jump			UMETA(DisplayName = "Jump"),
	
	Laser			UMETA(DisplayName = "Laser"),

	Grab			UMETA(DisplayName = "Grab"),

	Struggle		UMETA(DisplayName = "Struggle"),

	Testing			UMETA(DisplayName = "Testing"),
};
