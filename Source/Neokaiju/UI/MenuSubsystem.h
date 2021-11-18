// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "MenuSubsystem.generated.h"

class UNavigatableMenu;
class UUserWidget;
class UMenuParent;

UCLASS()
class UNavigationBreadcrumb : public UObject
{
	GENERATED_BODY()

public:
	static UNavigationBreadcrumb* Create(UObject* WorldContextObject, TSubclassOf<UNavigatableMenu> MenuClassValue, UMenuParent* MenuParent);

	UFUNCTION()
		void AddInstance(UObject* WorldContextObject, UMenuParent* MenuParent);

	UFUNCTION()
		void RemoveInstance();

	UPROPERTY()
		UNavigatableMenu* MenuInstance;

	UPROPERTY()
		TSubclassOf<UNavigatableMenu> MenuClass;
};
/**
 * 
 */
UCLASS()
class NEOKAIJU_API UMenuSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
		void NavigateTo(TSubclassOf<UNavigatableMenu> MenuClass, bool bRemoveCurrentFromViewport = true);

	UFUNCTION(BlueprintCallable)
		void NavigateBack();

	UFUNCTION(BlueprintCallable)
		void ClearNavigationBreadcrumbs(bool bRemoveAllFromViewport = true);

	UFUNCTION(BlueprintCallable, BlueprintPure)
		UNavigationBreadcrumb* GetCurrentBreadcrumb() const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
		UNavigatableMenu* GetCurrentMenu() const;

	UFUNCTION(BlueprintCallable)
		void Reset(UMenuParent* MenuParent);

	UFUNCTION(BlueprintCallable, BlueprintPure)
		int32 GetBreadcrumbCount() const;

protected:
	UPROPERTY()
		TArray<UNavigationBreadcrumb*> NavigationBreadcrumbs;

	UPROPERTY()
		UMenuParent* CurrentMenuParent;
};
