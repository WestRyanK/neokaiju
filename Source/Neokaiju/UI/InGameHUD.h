#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"

#include "Components/WidgetComponent.h"
#include <Neokaiju\Structures\BreakableStructures\BreakableStructureSubsystem.h>
#include <Neokaiju\KaijuCharacter\KaijuCharacterEventSubsystem.h>
#include <Neokaiju\UI\WorldGenerated\CumulativePointPopup.h>
#include <Neokaiju\KaijuCharacter\KaijuScoreState.h>
#include <Engine.h>


#include "InGameHUD.generated.h"

/**
 *
 */
UCLASS()
class NEOKAIJU_API AInGameHUD : public AHUD
{
	GENERATED_BODY()

public:
	// Primary draw call for the HUD.
	virtual void BeginPlay() override;
	UFUNCTION(BlueprintCallable)
		void ClearActivePointPopup();
protected:
	UFUNCTION(BlueprintImplementableEvent)
		void CreateBlockPointPopup(ABreakableStructure* DestroyedStructure);
	UFUNCTION(BlueprintImplementableEvent)
		void AddClawEscapePrompt();
	UFUNCTION(BlueprintImplementableEvent)
		void RemoveClawEscapePrompt();
	UFUNCTION(BlueprintCallable)
		void UpdatePointPopup(ABreakableStructure* BreakableStructure);
	UPROPERTY(BlueprintReadWrite)
		UCumulativePointPopup* activePointPopup;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TSubclassOf<UCumulativePointPopup> pointPopupClass;
};