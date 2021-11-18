// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Neokaiju/UI/NavigatableInterface.h"
#include "NavigatableMenu.generated.h"

class UMenuSubsystem;
/**
 *
 */
UCLASS()
class NEOKAIJU_API UNavigatableMenu : public UUserWidget, public INavigatableInterface
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;

	UFUNCTION(BlueprintCallable)
		void UnfocusWidgets();

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		float MinAxisMagnitudeToMove = 0.5f;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

	UFUNCTION(BlueprintCallable)
		void FocusInitialWidget();

protected:
	UFUNCTION()
		UWidget* GetFocusedWidget();

	void BindMenuInput();

	UFUNCTION()
		void UIControlPressedHandler();

	UMenuSubsystem* GetMenuSubsystem();

	float RefocusDelay = 0.5f;

	float LastFocusTime;

	FInputActionBinding UIControlBinding;

	UFUNCTION()
		void MenuUpPressedHandler(float AxisValue);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void OnMenuUpPressed(float AxisValue);

	FInputAxisBinding MenuUpBinding;

	UFUNCTION()
		void MenuRightPressedHandler(float AxisValue);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void OnMenuRightPressed(float AxisValue);

	FInputAxisBinding MenuRightBinding;
};
