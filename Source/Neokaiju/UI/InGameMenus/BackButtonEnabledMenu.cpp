// Fill out your copyright notice in the Description page of Project Settings.


#include "BackButtonEnabledMenu.h"
#include "Kismet/GameplayStatics.h"
#include "Neokaiju/UI/MenuSubsystem.h"
#include "Engine/World.h"

void UBackButtonEnabledMenu::BackButtonPressed()
{
	UWorld* World = this->GetWorld();
	if (IsValid(World))
	{
		UGameInstance* GameInstance = World->GetGameInstance();
		if (IsValid(GameInstance))
		{
			UMenuSubsystem* MenuSubsystem = GameInstance->GetSubsystem<UMenuSubsystem>();
			if (IsValid(MenuSubsystem))
			{
				MenuSubsystem->NavigateBack();
			}
		}
	}
}

void UBackButtonEnabledMenu::NativeConstruct()
{
	Super::NativeConstruct();

	APlayerController* Controller = UGameplayStatics::GetPlayerController(this, 0);
	if (IsValid(Controller) && IsValid(Controller->InputComponent))
	{
		this->BackBinding = FInputActionBinding("Back", EInputEvent::IE_Pressed);
		this->BackBinding.bConsumeInput = false;
		this->BackBinding.bExecuteWhenPaused = true;
		this->BackBinding.ActionDelegate.BindDelegate(this, &UBackButtonEnabledMenu::BackHandler);
		Controller->InputComponent->AddActionBinding(this->BackBinding);
	}
}

void UBackButtonEnabledMenu::NativeDestruct()
{
	Super::NativeDestruct();

	APlayerController* Controller = UGameplayStatics::GetPlayerController(this, 0);
	if (IsValid(Controller) && IsValid(Controller->InputComponent))
	{
		Controller->InputComponent->RemoveActionBindingForHandle(this->BackBinding.GetHandle());
	}
}

void UBackButtonEnabledMenu::BackHandler()
{
	this->OnBackPressed();
}

