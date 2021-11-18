#include "InGameHUD.h"

void AInGameHUD::ClearActivePointPopup() {
	activePointPopup = NULL;
}

void AInGameHUD::BeginPlay()
{
	UBreakableStructureSubsystem* BreakableStructureSubsystem = this->GetWorld()->GetSubsystem<UBreakableStructureSubsystem>();
	UKaijuCharacterEventSubsystem* KaijuCharacterEventSubsystem = this->GetWorld()->GetSubsystem<UKaijuCharacterEventSubsystem>();
	BreakableStructureSubsystem->OnStructureDestroyed.AddUniqueDynamic(this, &AInGameHUD::CreateBlockPointPopup);
	KaijuCharacterEventSubsystem->OnKaijuTrappedByClaw.AddUniqueDynamic(this, &AInGameHUD::AddClawEscapePrompt);
	KaijuCharacterEventSubsystem->OnKaijuReleasedFromClaw.AddUniqueDynamic(this, &AInGameHUD::RemoveClawEscapePrompt);
	Super::BeginPlay();
}

void AInGameHUD::UpdatePointPopup(ABreakableStructure* BreakableStructure) {

	APlayerController* playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	AKaijuScoreState* scoreState = Cast<AKaijuScoreState>(playerController->PlayerState);
	UAbilitySystemComponent* ASC = BreakableStructure->AbilitySystemComponent;
	float PropertyValue = ASC->GetNumericAttribute(UBreakableStructureAttributeSet::GetStructureValueAttribute());

	if (activePointPopup && activePointPopup->multiplier == scoreState->GetTimeBonusMultiplier()) {
		activePointPopup->AddToScore(PropertyValue);
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, FString::Printf(TEXT("Creating new widget x%d"), scoreState->GetTimeBonusMultiplier()));
		if (activePointPopup) {
			activePointPopup->StylizedRemoveFromViewport();
		}
		activePointPopup = CreateWidget<UCumulativePointPopup>(UGameplayStatics::GetPlayerController(GetWorld(), 0), pointPopupClass);

		if (IsValid(activePointPopup)) {
			activePointPopup->Setup(scoreState->GetTimeBonusMultiplier(), PropertyValue);
		}
	}
}