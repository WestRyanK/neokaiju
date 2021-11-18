// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "NeokaijuPlayerStart.generated.h"


class UBillboardComponent;
/**
 * 
 */
UCLASS()
class NEOKAIJU_API ANeokaijuPlayerStart : public APlayerStart
{
	GENERATED_BODY()
	
public:
	ANeokaijuPlayerStart(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "NeokaijuPlayerStart")
		float CapsuleHalfHeight = 40.0f;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "NeokaijuPlayerStart")
		float CapsuleRadius = 40.0f;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "NeokaijuPlayerStart")
		bool bIsCurrentPlayerStart = false;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "NeokaijuPlayerStart")
		bool bShouldSnapToZeroY = true;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		UBillboardComponent* CurrentPlayerStartSprite;

	virtual void OnConstruction(const FTransform& Transform) override;

	UFUNCTION(BlueprintCallable, CallInEditor, Category = "NeokaijuPlayerStart")
		void SetAsPlayerStart();

	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (WorldContext = "WorldContextObject"))
		static APlayerStart* GetCurrentPlayerStart(UObject* WorldContextObject);

private:
	void SetIsCurrentPlayerStart(bool bIsCurrentPlayerStartValue);
};
