// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuSubsystem.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Neokaiju/UI/NavigatableMenu.h"
#include "Neokaiju/UI/MenuParent.h"
#include "Components/PanelSlot.h"
#include "Kismet/GameplayStatics.h"

void UMenuSubsystem::NavigateTo(TSubclassOf<UNavigatableMenu> MenuClass, bool bRemoveCurrentFromViewport)
{
	if (bRemoveCurrentFromViewport && this->NavigationBreadcrumbs.Num() > 0)
	{
		UNavigationBreadcrumb* CurrentBreadcrumb = this->NavigationBreadcrumbs.Last();
		CurrentBreadcrumb->RemoveInstance();
	}

	this->NavigationBreadcrumbs.Add(UNavigationBreadcrumb::Create(this, MenuClass, this->CurrentMenuParent));
}

void UMenuSubsystem::NavigateBack()
{
	UNavigationBreadcrumb* BreadcrumbToPop = this->GetCurrentBreadcrumb();
	if (IsValid(BreadcrumbToPop))
	{
		this->NavigationBreadcrumbs.Pop();
		BreadcrumbToPop->RemoveInstance();

		UNavigationBreadcrumb* NextBreadcrumb = this->GetCurrentBreadcrumb();
		if (IsValid(NextBreadcrumb))
		{
			if (!IsValid(NextBreadcrumb->MenuInstance))
			{
				NextBreadcrumb->AddInstance(this, this->CurrentMenuParent);
			}

			NextBreadcrumb->MenuInstance->FocusInitialWidget();
		}
	}
}

int32 UMenuSubsystem::GetBreadcrumbCount() const
{
	return this->NavigationBreadcrumbs.Num();
}

UNavigationBreadcrumb* UMenuSubsystem::GetCurrentBreadcrumb() const
{
	if (this->NavigationBreadcrumbs.Num() > 0)
	{
		return this->NavigationBreadcrumbs.Last();
	}
	else
	{
		return nullptr;
	}
}

UNavigatableMenu* UMenuSubsystem::GetCurrentMenu() const
{
	UNavigationBreadcrumb* Breadcrumb = this->GetCurrentBreadcrumb();
	if (IsValid(Breadcrumb))
	{
		return Breadcrumb->MenuInstance;
	}
	return nullptr;
}


void UMenuSubsystem::ClearNavigationBreadcrumbs(bool bRemoveAllFromViewport)
{
	if (bRemoveAllFromViewport)
	{
		for (UNavigationBreadcrumb* Breadcrumb : this->NavigationBreadcrumbs)
		{
			if (IsValid(Breadcrumb))
			{
				Breadcrumb->RemoveInstance();
			}
		}
	}

	this->NavigationBreadcrumbs.Empty();
}

void UMenuSubsystem::Reset(UMenuParent* MenuParent)
{
	this->ClearNavigationBreadcrumbs(true);
	this->CurrentMenuParent = MenuParent;
}





UNavigationBreadcrumb* UNavigationBreadcrumb::Create(UObject* WorldContextObject, TSubclassOf<UNavigatableMenu> MenuClassValue, UMenuParent* MenuParent)
{
	UNavigationBreadcrumb* Breadcrumb = NewObject<UNavigationBreadcrumb>();
	Breadcrumb->MenuClass = MenuClassValue;
	Breadcrumb->AddInstance(WorldContextObject, MenuParent);

	return Breadcrumb;
}

void UNavigationBreadcrumb::AddInstance(UObject* WorldContextObject, UMenuParent* MenuParent)
{
	APlayerController* Controller = UGameplayStatics::GetPlayerController(WorldContextObject, 0);
	this->MenuInstance = Cast<UNavigatableMenu>(UWidgetBlueprintLibrary::Create(WorldContextObject, this->MenuClass, Controller));
	if (IsValid(this->MenuInstance))
	{
		if (IsValid(MenuParent))
		{
			MenuParent->AddMenu(this->MenuInstance);
		}
		else
		{
			this->MenuInstance->AddToViewport();
		}
	}
}

void UNavigationBreadcrumb::RemoveInstance()
{
	if (IsValid(this->MenuInstance))
	{
		this->MenuInstance->RemoveFromViewport();
		this->MenuInstance = nullptr;
	}
}
