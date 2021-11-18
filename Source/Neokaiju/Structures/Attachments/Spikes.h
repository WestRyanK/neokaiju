// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Neokaiju/Structures/Attachments/Attachment.h"
#include "Neokaiju/KaijuCharacter/KaijuCharacter.h"
#include "Spikes.generated.h"

/**
 * 
 */
UCLASS()
class NEOKAIJU_API ASpikes : public AAttachment
{
	GENERATED_BODY()

public:

	ASpikes();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Component")
		class UBoxComponent* SpikeHitBox;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Component")
		class UBoxComponent* SpikeBlockBox;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Component")
		FTimerHandle timerHandle;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Component")
		class TSubclassOf<class UGameplayEffect> SpikeDamageEffect;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
		void OnTouch(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
		void CheckForOverlaps();
};
