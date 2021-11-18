// Fill out your copyright notice in the Description page of Project Settings.


#include "NeokaijuPlayerStart.h"
#include "Components/CapsuleComponent.h"
#include "Components/BillboardComponent.h"
#include "Kismet/GameplayStatics.h"

ANeokaijuPlayerStart::ANeokaijuPlayerStart(const FObjectInitializer& ObjectInitializer) : APlayerStart(ObjectInitializer)
{
	this->GetCapsuleComponent()->SetCapsuleHalfHeight(this->CapsuleHalfHeight);
	this->GetCapsuleComponent()->SetCapsuleRadius(this->CapsuleRadius);

	this->CurrentPlayerStartSprite = CreateDefaultSubobject<UBillboardComponent>("CurrentPlayerStartSprite");
	this->CurrentPlayerStartSprite->SetupAttachment(this->RootComponent);
}

void ANeokaijuPlayerStart::SetAsPlayerStart()
{
	TArray<AActor*> PlayerStarts;
	UGameplayStatics::GetAllActorsOfClass(this, ANeokaijuPlayerStart::StaticClass(), PlayerStarts);

	for (AActor* PlayerStart : PlayerStarts)
	{
		ANeokaijuPlayerStart* NeokaijuPlayerStart = Cast<ANeokaijuPlayerStart>(PlayerStart);
		if (IsValid(NeokaijuPlayerStart))
		{
			NeokaijuPlayerStart->SetIsCurrentPlayerStart(false);
		}
	}

	this->SetIsCurrentPlayerStart(true);
}

void ANeokaijuPlayerStart::OnConstruction(const FTransform& Transform)
{
	this->SetIsCurrentPlayerStart(this->bIsCurrentPlayerStart); // Trigger Visible Change
	FVector Location = this->GetActorLocation();
	if (Location.Y != 0 && this->bShouldSnapToZeroY)
	{
		Location.Y = 0;
		this->SetActorLocation(Location);
	}
}

void ANeokaijuPlayerStart::SetIsCurrentPlayerStart(bool bIsCurrentPlayerStartValue)
{
	this->bIsCurrentPlayerStart = bIsCurrentPlayerStartValue;
	if (IsValid(this->CurrentPlayerStartSprite))
	{
		this->CurrentPlayerStartSprite->SetVisibility(this->bIsCurrentPlayerStart);
	}
}


APlayerStart* ANeokaijuPlayerStart::GetCurrentPlayerStart(UObject* WorldContextObject)
{
	if (IsValid(WorldContextObject))
	{
		// Try to get the NeokaijuPlayerStart that is set as current.
		TArray<AActor*> PlayerStarts;
		UGameplayStatics::GetAllActorsOfClass(WorldContextObject, ANeokaijuPlayerStart::StaticClass(), PlayerStarts);

		for (AActor* PlayerStart : PlayerStarts)
		{
			ANeokaijuPlayerStart* NeokaijuPlayerStart = Cast<ANeokaijuPlayerStart>(PlayerStart);
			if (IsValid(NeokaijuPlayerStart) && NeokaijuPlayerStart->bIsCurrentPlayerStart)
			{
				return NeokaijuPlayerStart;
			}
		}

		// If none are set to current, get the first NeokaijuPlayerStart
		if (PlayerStarts.Num() > 0)
		{
			return Cast<ANeokaijuPlayerStart>(PlayerStarts[0]);
		}

		// If there are no NeokaijuPlayerStarts, return a standard PlayerStart
		UGameplayStatics::GetAllActorsOfClass(WorldContextObject, APlayerStart::StaticClass(), PlayerStarts);
		if (PlayerStarts.Num() > 0)
		{
			return Cast<APlayerStart>(PlayerStarts[0]);
		}

	}

	// If there are no player starts, we're out of luck!
	return nullptr;
}
