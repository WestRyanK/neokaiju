// Fill out your copyright notice in the Description page of Project Settings.


#include "DebugCommandComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/Actor.h"
#include "Neokaiju/KaijuCharacter/KaijuCharacter.h"
#include "Neokaiju/KaijuCharacter/KaijuScoreState.h"
#include "Neokaiju/Ability/StandardAbilities/LevelChangeAbility/LevelChangeAbility.h"
#include "Neokaiju/Core/PhasedGameMode.h"
#include "AbilitySystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "Engine/Engine.h"

// Sets default values for this component's properties
UDebugCommandComponent::UDebugCommandComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void UDebugCommandComponent::BeginPlay()
{
	Super::BeginPlay();

#if !UE_BUILD_SHIPPING // Don't allow debug commands in shipping!
	this->GetOwner()->InputComponent->BindKey(EKeys::One, EInputEvent::IE_Pressed, this, &UDebugCommandComponent::Slomo1_0);
	this->GetOwner()->InputComponent->BindKey(EKeys::Two, EInputEvent::IE_Pressed, this, &UDebugCommandComponent::Slomo0_5);
	this->GetOwner()->InputComponent->BindKey(EKeys::Three, EInputEvent::IE_Pressed, this, &UDebugCommandComponent::Slomo0_2);
	this->GetOwner()->InputComponent->BindKey(EKeys::Four, EInputEvent::IE_Pressed, this, &UDebugCommandComponent::Slomo0_1);
	this->GetOwner()->InputComponent->BindKey(EKeys::LeftBracket, EInputEvent::IE_Pressed, this, &UDebugCommandComponent::HideAbilitySystem);
	this->GetOwner()->InputComponent->BindKey(EKeys::RightBracket, EInputEvent::IE_Pressed, this, &UDebugCommandComponent::ShowAbilitySystem);
	this->GetOwner()->InputComponent->BindKey(EKeys::M, EInputEvent::IE_Pressed, this, &UDebugCommandComponent::InstantDamage);
	this->GetOwner()->InputComponent->BindKey(EKeys::Period, EInputEvent::IE_Pressed, this, &UDebugCommandComponent::InstantKill);
	this->GetOwner()->InputComponent->BindKey(EKeys::Comma, EInputEvent::IE_Pressed, this, &UDebugCommandComponent::InstantWin);
	this->GetOwner()->InputComponent->BindKey(EKeys::Backslash, EInputEvent::IE_Pressed, this, &UDebugCommandComponent::ToggleAchievements);
	this->GetOwner()->InputComponent->BindKey(FInputChord(EKeys::LeftBracket, EModifierKey::Shift), EInputEvent::IE_Pressed, this, &UDebugCommandComponent::PreviousAbilityTarget);
	this->GetOwner()->InputComponent->BindKey(FInputChord(EKeys::RightBracket, EModifierKey::Shift), EInputEvent::IE_Pressed, this, &UDebugCommandComponent::NextAbilityTarget);
	this->GetOwner()->InputComponent->BindKey(EKeys::Hyphen, EInputEvent::IE_Pressed, this, &UDebugCommandComponent::LevelDown);
	this->GetOwner()->InputComponent->BindKey(EKeys::Equals, EInputEvent::IE_Pressed, this, &UDebugCommandComponent::LevelUp);
	this->GetOwner()->InputComponent->BindKey(EKeys::BackSpace, EInputEvent::IE_Pressed, this, &UDebugCommandComponent::TopUp);
	this->GetOwner()->InputComponent->BindKey(EKeys::RightShift, EInputEvent::IE_Pressed, this, &UDebugCommandComponent::SaveBabyKaiju);
	this->GetOwner()->InputComponent->BindKey(EKeys::Enter, EInputEvent::IE_Pressed, this, &UDebugCommandComponent::DestroyWeapon);
	this->GetOwner()->InputComponent->BindKey(EKeys::Slash, EInputEvent::IE_Pressed, this, &UDebugCommandComponent::DebugHelp);
#endif

}

void UDebugCommandComponent::DisplayMessage(FString Message, float Duration)
{
	if (IsValid(GEngine))
	{
		GEngine->AddOnScreenDebugMessage(-1, Duration, FColor::Orange, Message);
	}
}

void UDebugCommandComponent::ExecCommand(FString Command)
{
	UGameplayStatics::GetPlayerController(this, 0)->ConsoleCommand(Command);
	this->DisplayMessage(Command);
}


void UDebugCommandComponent::Slomo1_0()
{
	this->ExecCommand(TEXT("slomo 1"));
}

void UDebugCommandComponent::Slomo0_5()
{
	this->ExecCommand(TEXT("slomo 0.5"));
}

void UDebugCommandComponent::Slomo0_2()
{
	this->ExecCommand(TEXT("slomo 0.2"));
}

void UDebugCommandComponent::Slomo0_1()
{
	this->ExecCommand(TEXT("slomo 0.1"));
}

void UDebugCommandComponent::ShowAbilitySystem()
{
	if (!this->bIsAbilitySystemShown)
	{
		this->ExecCommand(TEXT("showdebug abilitysystem"));
	}
	else
	{
		this->ExecCommand(TEXT("AbilitySystem.Debug.NextCategory"));
	}
	this->bIsAbilitySystemShown = true;
}

void UDebugCommandComponent::HideAbilitySystem()
{
	if (this->bIsAbilitySystemShown)
	{
		this->ExecCommand(TEXT("showdebug"));
		this->bIsAbilitySystemShown = false;
	}
}

void UDebugCommandComponent::PreviousAbilityTarget()
{
	if (this->bIsAbilitySystemShown)
	{
		this->ExecCommand(TEXT("AbilitySystem.Debug.PrevTarget"));
	}
}
void UDebugCommandComponent::NextAbilityTarget()
{
	if (this->bIsAbilitySystemShown)
	{
		this->ExecCommand(TEXT("AbilitySystem.Debug.NextTarget"));
	}
}

void UDebugCommandComponent::LevelDown()
{
	AKaijuCharacter* Character = Cast<AKaijuCharacter>(this->GetOwner());
	Character->GetAbilitySystemComponent()->ApplyGameplayEffectToSelf(this->LevelDownEffect.GetDefaultObject(), 1, Character->GetAbilitySystemComponent()->MakeEffectContext());
	this->DisplayMessage(TEXT("Level Down"));
}

void UDebugCommandComponent::LevelUp()
{
	AKaijuCharacter* Character = Cast<AKaijuCharacter>(this->GetOwner());
	Character->GetAbilitySystemComponent()->ApplyGameplayEffectToSelf(this->LevelUpEffect.GetDefaultObject(), 1, Character->GetAbilitySystemComponent()->MakeEffectContext());
	this->DisplayMessage(TEXT("Level Up"));
}
void UDebugCommandComponent::TopUp()
{
	AKaijuCharacter* Character = Cast<AKaijuCharacter>(this->GetOwner());
	float KaijuLevel = Character->GetKaijuLevel();
	Character->GetAbilitySystemComponent()->ApplyGameplayEffectToSelf(this->TopUpEffect.GetDefaultObject(), KaijuLevel, Character->GetAbilitySystemComponent()->MakeEffectContext());
	this->DisplayMessage(TEXT("Top Up Health and Laser"));
}

void UDebugCommandComponent::InstantDamage()
{
	AKaijuCharacter* Character = Cast<AKaijuCharacter>(this->GetOwner());
	float KaijuLevel = Character->GetKaijuLevel();
	Character->GetAbilitySystemComponent()->ApplyGameplayEffectToSelf(this->InstantDamageEffect.GetDefaultObject(), KaijuLevel, Character->GetAbilitySystemComponent()->MakeEffectContext());
	this->DisplayMessage(TEXT("Instant Damage"));
}

void UDebugCommandComponent::InstantKill()
{
	AKaijuCharacter* Character = Cast<AKaijuCharacter>(this->GetOwner());
	float KaijuLevel = Character->GetKaijuLevel();
	Character->GetAbilitySystemComponent()->ApplyGameplayEffectToSelf(this->InstantKillEffect.GetDefaultObject(), KaijuLevel, Character->GetAbilitySystemComponent()->MakeEffectContext());
	this->DisplayMessage(TEXT("Instant Kill"));
}

void UDebugCommandComponent::InstantWin()
{
	APhasedGameMode* GameMode = Cast<APhasedGameMode>(this->GetWorld()->GetAuthGameMode());
	if (IsValid(GameMode))
	{
		GameMode->BeginPostPlayPhase(EGameModePlayResult::Success);
		this->DisplayMessage(TEXT("Instant Win"));
	}
}

void UDebugCommandComponent::SaveBabyKaiju()
{
	AKaijuCharacter* Character = Cast<AKaijuCharacter>(this->GetOwner());
	AKaijuScoreState* State = Cast<AKaijuScoreState>(Character->GetPlayerState());
	State->SetBabyKaijusSaved(State->GetBabyKaijusSaved() + 1);
	this->DisplayMessage(TEXT("Save Baby Kaiju"));
}

void UDebugCommandComponent::DestroyWeapon()
{
	AKaijuCharacter* Character = Cast<AKaijuCharacter>(this->GetOwner());
	AKaijuScoreState* State = Cast<AKaijuScoreState>(Character->GetPlayerState());
	this->DisplayMessage(TEXT("Destroy Weapon"));
}

void UDebugCommandComponent::ShowAchievements()
{
	if (IsValid(this->AchievementDebugWidgetType) && !IsValid(this->SpawnedAchievementDebugWidget))
	{
		this->SpawnedAchievementDebugWidget = CreateWidget<UUserWidget>(this->GetWorld(), this->AchievementDebugWidgetType);
		this->SpawnedAchievementDebugWidget->AddToViewport();
	}
}

void UDebugCommandComponent::HideAchievements()
{
	if (IsValid(this->SpawnedAchievementDebugWidget))
	{
		this->SpawnedAchievementDebugWidget->RemoveFromViewport();
		this->SpawnedAchievementDebugWidget = nullptr;
	}
}

void UDebugCommandComponent::ToggleAchievements()
{
	if (IsValid(this->SpawnedAchievementDebugWidget))
	{
		this->HideAchievements();
	}
	else
	{
		this->ShowAchievements();
	}
}

void UDebugCommandComponent::DebugHelp()
{
	this->DisplayMessage(TEXT("?          Debug Help"), 5);
	this->DisplayMessage(TEXT("Enter      Destroy Weapon"), 5);
	this->DisplayMessage(TEXT("RShift     Save Baby Kaiju"), 5);
	this->DisplayMessage(TEXT("Backspace  Top Up Health and Laser"), 5);
	this->DisplayMessage(TEXT("M          Instant Damage"), 5);
	this->DisplayMessage(TEXT(".          Instant Kill"), 5);
	this->DisplayMessage(TEXT(",          Instant Win"), 5);
	this->DisplayMessage(TEXT(";          Debug/Free Camera"), 5);
	this->DisplayMessage(TEXT("\\          Toggle Achievement Info"), 5);
	this->DisplayMessage(TEXT("-          Level Down"), 5);
	this->DisplayMessage(TEXT("+          Level Up"), 5);
	this->DisplayMessage(TEXT("{          Previous AbilitySystemTarget"), 5);
	this->DisplayMessage(TEXT("}          Next AbilitySystemTarget"), 5);
	this->DisplayMessage(TEXT("[          Hide Ability Debug"), 5);
	this->DisplayMessage(TEXT("]          Show Ability Debug/Cycle Ability Pages"), 5);
	this->DisplayMessage(TEXT("4          Slomo 0.1"), 5);
	this->DisplayMessage(TEXT("3          Slomo 0.2"), 5);
	this->DisplayMessage(TEXT("2          Slomo 0.5 (Half Speed)"), 5);
	this->DisplayMessage(TEXT("1          Slomo 1.0 (Normal Speed)"), 5);
}
