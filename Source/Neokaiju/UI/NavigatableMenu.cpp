// Fill out your copyright notice in the Description page of Project Settings.


#include "NavigatableMenu.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Neokaiju/UI/MenuSubsystem.h"
#include "Kismet/GameplayStatics.h"

void UNavigatableMenu::NativeConstruct()
{
	Super::NativeConstruct();

	UMenuSubsystem* MenuSubsystem = this->GetMenuSubsystem();
	if (MenuSubsystem->GetBreadcrumbCount() == 0)
	{
		APlayerController* Controller = UGameplayStatics::GetPlayerController(this, 0);
		if (IsValid(Controller))
		{
			UWidgetBlueprintLibrary::SetInputMode_GameAndUI(Controller, this, false, false);
			Controller->bShowMouseCursor = true;
		}
	}

	this->FocusInitialWidget();
	this->BindMenuInput();
}

void UNavigatableMenu::NativeDestruct()
{
	Super::NativeDestruct();

	APlayerController* Controller = UGameplayStatics::GetPlayerController(this, 0);
	if (IsValid(Controller) && IsValid(Controller->InputComponent))
	{
		Controller->InputComponent->RemoveActionBindingForHandle(this->UIControlBinding.GetHandle());
		this->MenuUpBinding.AxisDelegate.Unbind();
		Controller->InputComponent->AxisBindings.Pop();
		//this->MenuRightBinding.AxisDelegate.Unbind();

		UMenuSubsystem* MenuSubsystem = this->GetMenuSubsystem();
		if (MenuSubsystem->GetBreadcrumbCount() == 0)
		{
			UWidgetBlueprintLibrary::SetInputMode_GameOnly(Controller);
			Controller->bShowMouseCursor = false;
			//UWidgetBlueprintLibrary::SetFocusToGameViewport();
		}
	}
}

void UNavigatableMenu::BindMenuInput()
{
	APlayerController* Controller = UGameplayStatics::GetPlayerController(this, 0);
	if (IsValid(Controller) && IsValid(Controller->InputComponent))
	{
		this->UIControlBinding = FInputActionBinding("UIControl", EInputEvent::IE_Pressed);
		this->UIControlBinding.bConsumeInput = false;
		this->UIControlBinding.bExecuteWhenPaused = true;
		this->UIControlBinding.ActionDelegate.BindDelegate(this, &UNavigatableMenu::UIControlPressedHandler);
		Controller->InputComponent->AddActionBinding(this->UIControlBinding);

		this->MenuUpBinding = Controller->InputComponent->BindAxis("MenuUp", this, &UNavigatableMenu::MenuUpPressedHandler);
		this->MenuUpBinding.bConsumeInput = false;
		this->MenuUpBinding.bExecuteWhenPaused = true;

		//this->MenuRightBinding = Controller->InputComponent->BindAxis("MenuRight", this, &UNavigatableMenu::MenuRightPressedHandler);
		//this->MenuRightBinding.bConsumeInput = false;
		//this->MenuRightBinding.bExecuteWhenPaused = true;
	}
}

void UNavigatableMenu::MenuUpPressedHandler(float AxisValue)
{
	this->OnMenuUpPressed(AxisValue);
}

void UNavigatableMenu::MenuRightPressedHandler(float AxisValue)
{
	this->OnMenuRightPressed(AxisValue);
}

UMenuSubsystem* UNavigatableMenu::GetMenuSubsystem()
{
	UWorld* World = this->GetWorld();
	if (IsValid(World))
	{
		UGameInstance* GameInstance = World->GetGameInstance();
		if (IsValid(GameInstance))
		{
			UMenuSubsystem* MenuSubsystem = GameInstance->GetSubsystem<UMenuSubsystem>();
			return MenuSubsystem;
		}
	}
	return nullptr;
}

void UNavigatableMenu::UIControlPressedHandler()
{
	UMenuSubsystem* MenuSubsystem = this->GetMenuSubsystem();
	if (IsValid(MenuSubsystem))
	{
		UNavigatableMenu* CurrentMenu = MenuSubsystem->GetCurrentMenu();

		bool bIsOtherMenuCurrent = IsValid(CurrentMenu) && this != CurrentMenu;

		if (!bIsOtherMenuCurrent && !IsValid(this->GetFocusedWidget()))
		{
			this->FocusInitialWidget();
		}
	}
}

void UNavigatableMenu::FocusInitialWidget()
{
	UWidget* FocusedWidget = this->Execute_GetInitiallyFocusedWidget(this);
	if (IsValid(FocusedWidget))
	{
		FocusedWidget->SetKeyboardFocus();
	}
}

UWidget* UNavigatableMenu::GetFocusedWidget()
{
	TArray<UWidget*> FocusableWidgets;
	this->Execute_GetFocusableWidgets(this, FocusableWidgets);

	for (UWidget* FocusableWidget : FocusableWidgets)
	{
		if (IsValid(FocusableWidget) && FocusableWidget->HasFocusedDescendants())
		{
			return FocusableWidget;
		}
	}

	return nullptr;
}

void UNavigatableMenu::UnfocusWidgets()
{
	UWidgetBlueprintLibrary::SetFocusToGameViewport();
}

void UNavigatableMenu::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	//if (this->IsVisible())
	//{
	//	UWidget* FocusedWidget = this->GetFocusedWidget();

	//	if (IsValid(FocusedWidget))
	//	{
	//		this->LastFocusTime = this->GetWorld()->GetTimeSeconds();
	//	}
	//	else if (this->GetWorld()->GetTimeSeconds() - this->LastFocusTime > this->RefocusDelay)
	//	{
	//		this->FocusInitialWidget();
	//	}
	//}
}
