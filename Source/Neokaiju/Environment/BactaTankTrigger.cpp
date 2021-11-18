// Fill out your copyright notice in the Description page of Project Settings.


#include "BactaTankTrigger.h"
#include "Neokaiju/Structures/BreakableStructures/BreakableStructure.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Neokaiju/KaijuCharacter/Components/ShakerComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Neokaiju/Core/PlayLevelGameMode.h"

void ABactaTankTrigger::OnBeginTrigger(AActor* TriggeringActorValue)
{
	Super::OnBeginTrigger(TriggeringActorValue);

	if (IsValid(this->TriggeringActor))
	{
		ACharacter* Character = Cast<ACharacter>(this->TriggeringActor);
		if (IsValid(Character))
		{
			Character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
		}


		this->FloatingShaker = NewObject<UShakerComponent>(this->TriggeringActor);
		this->FloatingShaker->RegisterComponent();
		this->FloatingShaker->ShakeClass = this->FloatingShakeClass;
		this->FloatingShaker->PlayShakeOnActor();
	}
}

void ABactaTankTrigger::WaitForInputPressed()
{
	Super::WaitForInputPressed();

	if (IsValid(this->FloatingShaker))
	{
		this->FloatingShaker->EndShake();
		this->FloatingShaker->DestroyComponent();
	}

	if (IsValid(this->BactaTankGlass))
	{
		this->BactaTankGlass->AttributeSet->SetHealth(0);
	}

	ACharacter* Character = Cast<ACharacter>(this->TriggeringActor);
	if (IsValid(Character))
	{
		Character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Falling);
	}

	AGameModeBase* GameMode = UGameplayStatics::GetGameMode(GetWorld());
	APlayLevelGameMode* LevelGameMode = Cast<APlayLevelGameMode>(GameMode);
	if (LevelGameMode)
	{
		LevelGameMode->StartMusicAndTimer();
	}
	
}
