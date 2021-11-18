// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayLevelGameMode.h"
#include "Neokaiju/KaijuCharacter/KaijuCharacterEventSubsystem.h"
#include "Neokaiju/KaijuCharacter/KaijuCharacterController.h"
#include "Neokaiju/Core/BaseCountdownTimer.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/UserWidget.h"
#include "TimerManager.h"
#include "Engine/World.h"

APlayLevelGameMode::APlayLevelGameMode()
{
}

void APlayLevelGameMode::BeginPlay()
{
	UKaijuCharacterEventSubsystem* KaijuCharacterEventSubsystem = this->GetWorld()->GetSubsystem<UKaijuCharacterEventSubsystem>();
	KaijuCharacterEventSubsystem->OnKaijuDied.AddUniqueDynamic(this, &APlayLevelGameMode::KaijuDied);

	APlayerController* Ctrl = UGameplayStatics::GetPlayerController(this, 0);
	AKaijuCharacterController* Controller = Cast<AKaijuCharacterController>(UGameplayStatics::GetPlayerController(this, 0));
	if (IsValid(Controller))
	{
		Controller->OnPauseUnpauseRequested.AddUniqueDynamic(this, &APlayLevelGameMode::PauseUnpauseRequested);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("KaijuCharacterController is null!"));
	}

	if (IsValid(this->CountdownTimerClass))
	{
		this->CountdownTimer = NewObject<UBaseCountdownTimer>(this, this->CountdownTimerClass);
	}

	Super::BeginPlay();
}

void APlayLevelGameMode::BeginPrePlayPhase()
{
	Super::BeginPrePlayPhase();

	this->DisableCharacterInput();
	UGameplayStatics::SetGamePaused(this, true);
	if (IsValid(this->PreGameMenu))
	{
		this->SpawnedPreGameMenu = CreateWidget<UUserWidget>(this->GetWorld(), this->PreGameMenu);
		this->SpawnedPreGameMenu->AddToViewport(10);
	}
}

void APlayLevelGameMode::PrePlayPhaseBegan_Implementation()
{

}

void APlayLevelGameMode::BeginPlayPhase()
{
	Super::BeginPlayPhase();
	UGameplayStatics::SetGamePaused(this, false);
	this->EnableCharacterInput();
	/*if (IsValid(this->CountdownTimer))
	{
		this->CountdownTimer->StartCountdown();
	}*/
}

void APlayLevelGameMode::EndPlay(const EEndPlayReason::Type Reason)
{
	UKaijuCharacterEventSubsystem* KaijuCharacterEventSubsystem = this->GetWorld()->GetSubsystem<UKaijuCharacterEventSubsystem>();
	if (IsValid(KaijuCharacterEventSubsystem))
	{
		KaijuCharacterEventSubsystem->OnKaijuDied.RemoveDynamic(this, &APlayLevelGameMode::KaijuDied);
	}

	Super::EndPlay(Reason);
}

void APlayLevelGameMode::KaijuDied()
{
	this->BeginPostPlayPhase(EGameModePlayResult::Failure);
}

void APlayLevelGameMode::PauseUnpauseRequested()
{
	if (this->CurrentPhase == EGameModePhase::PlayPhase)
	{
		if (!UGameplayStatics::IsGamePaused(this))
		{
			this->PauseGame();
		}
		else
		{
			this->UnpauseGame();
		}
	}
}

void APlayLevelGameMode::PauseGame()
{
	UE_LOG(LogTemp, Display, TEXT("PAUSE"));
	UGameplayStatics::SetGamePaused(this, true);
	if (IsValid(this->PauseMenu))
	{
		this->SpawnedPauseMenu = CreateWidget<UUserWidget>(this->GetWorld(), this->PauseMenu);
		this->SpawnedPauseMenu->AddToViewport();
	}
}

void APlayLevelGameMode::UnpauseGame()
{
	UE_LOG(LogTemp, Display, TEXT("UNPAUSE"));
	UGameplayStatics::SetGamePaused(this, false);
	if (IsValid(this->SpawnedPauseMenu))
	{
		this->SpawnedPauseMenu->RemoveFromViewport();
	}
}

void APlayLevelGameMode::BeginPostPlayPhase(EGameModePlayResult PlayResultValue)
{
	if (this->CurrentPhase != EGameModePhase::PostPlayPhase)
	{
		Super::BeginPostPlayPhase(PlayResultValue);
		FTimerHandle PostPlayTransitionTimerHandle;

		this->DisableCharacterInput();
		//APlayerController* Controller = UGameplayStatics::GetPlayerController(this, 0);
		//if (IsValid(Controller))
		//{
		//	UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(Controller);
		//}
		this->GetWorld()->GetTimerManager().SetTimer(PostPlayTransitionTimerHandle, this, &APlayLevelGameMode::DisplayPostPlayMenu, this->PostPlayPhaseTransitionDelay, false);
	}
}

void APlayLevelGameMode::DisplayPostPlayMenu()
{
	//UGameplayStatics::SetGamePaused(this, true);

	if (IsValid(this->PostGameMenu))
	{
		this->SpawnedPostGameMenu = CreateWidget<UUserWidget>(this->GetWorld(), this->PostGameMenu);
		this->SpawnedPostGameMenu->AddToViewport();
	}
}

bool APlayLevelGameMode::CanScoreBeChanged() const
{
	bool bCanScoreChange = (this->CurrentPhase == EGameModePhase::PlayPhase)
		|| (this->CurrentPhase == EGameModePhase::PostPlayPhase && this->bCanPostPlayAffectScore);
	return bCanScoreChange;
}

void APlayLevelGameMode::DisableCharacterInput()
{
	this->EnableCharacterInput();

	if (IsValid(this->DisableCharacterInputEffectClass))
	{
		ACharacter* Character = UGameplayStatics::GetPlayerCharacter(this, 0);
		IAbilitySystemInterface* ASCInterface = Cast<IAbilitySystemInterface>(Character);
		if (ASCInterface)
		{
			UAbilitySystemComponent* ASC = ASCInterface->GetAbilitySystemComponent();

			this->DisableCharacterInputEffectHandle = ASC->ApplyGameplayEffectToSelf(this->DisableCharacterInputEffectClass.GetDefaultObject(), 1, ASC->MakeEffectContext());
		}
	}
}

void APlayLevelGameMode::EnableCharacterInput()
{
	ACharacter* Character = UGameplayStatics::GetPlayerCharacter(this, 0);
	IAbilitySystemInterface* ASCInterface = Cast<IAbilitySystemInterface>(Character);
	if (ASCInterface)
	{
		UAbilitySystemComponent* ASC = ASCInterface->GetAbilitySystemComponent();
		if (this->DisableCharacterInputEffectHandle.IsValid())
		{
			ASC->RemoveActiveGameplayEffect(this->DisableCharacterInputEffectHandle);
			this->DisableCharacterInputEffectHandle.Invalidate();
		}
	}
}
