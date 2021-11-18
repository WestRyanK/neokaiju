// Fill out your copyright notice in the Description page of Project Settings.


#include "CaptureAbility.h"
#include "Neokaiju/KaijuCharacter/KaijuCharacter.h"
#include "Neokaiju/Ability/Core/Tasks/TickDurationAbilityTask.h"
#include "Neokaiju/Ability/Core/Tasks/InputPressedAbilityTask.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Neokaiju/Ability/EnvironmentAbilities/CaptureAbility/CapturerInterface.h"

UCaptureAbility::UCaptureAbility()
{
	this->InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerExecution;
}

bool UCaptureAbility::CaptureActor(TSubclassOf<UCaptureAbility> CaptureAbilitySubclass, AActor* Capturer, AActor* Captive, int32 NumRequiredStrugglesToEscape)
{
	TScriptInterface<IAbilitySystemInterface> ASCInterface = const_cast<AActor*>(Captive);
	if (ASCInterface && CaptureAbilitySubclass)
	{

		UAbilitySystemComponent* ASC = ASCInterface->GetAbilitySystemComponent();
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(CaptureAbilitySubclass);


		UCaptureAbility* Ability = Cast<UCaptureAbility>(AbilitySpec.Ability);
		bool bHasBlockedTags = ASC->HasAnyMatchingGameplayTags(Ability->BlockCapturingWhenCaptiveHasTags);
		if (bHasBlockedTags)
		{
			return false;
		}

		FGameplayAbilitySpecHandle AbilitySpecHandle = ASC->GiveAbility(AbilitySpec);
		bool bIsValid = AbilitySpecHandle.IsValid();
		FGameplayEventData Payload;
		Payload.EventTag = FGameplayTag::RequestGameplayTag(FName("Ability.Environment.Capture"));
		Payload.Instigator = Capturer;
		Payload.Target = Captive;
		Payload.EventMagnitude = NumRequiredStrugglesToEscape;

		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Captive, Payload.EventTag, Payload);
		return true;
	}
	return false;
}

void UCaptureAbility::ReleaseActor(AActor* Capturer, AActor* Captive)
{
	UCaptureAbility* CaptureInstance = UCaptureAbility::GetAbilityInstanceForCapturer(Capturer, Captive);
	if (IsValid(CaptureInstance))
	{
		CaptureInstance->CancelAbility(CaptureInstance->CurrentSpecHandle, CaptureInstance->CurrentActorInfo, CaptureInstance->CurrentActivationInfo, true);
	}
}

void UCaptureAbility::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	this->RemoveRequiredStrugglesToEscapeToCaptive(this->NumStrugglesToEscape);
	this->CheckStrugglesRemaining();
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

void UCaptureAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* Payload)
{
	if (!this->CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		this->EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, Payload);

	bool bIsOnlyCaptureForCapturer = !IsValid(UCaptureAbility::GetAbilityInstanceForCapturer(Payload->Instigator, Payload->Target));


	if (Payload && IsValid(Payload->Target) && IsValid(Payload->Instigator) && bIsOnlyCaptureForCapturer)
	{
		this->InitCaptureFromPayload(Payload);
		this->CaptureBegin(this->Captive);
	}
	else
	{
		if (!bIsOnlyCaptureForCapturer)
		{
			UE_LOG(LogTemp, Warning, TEXT("A Capturer can only capture a single actor at a time. Culprit: %s"), *Payload->Instigator->GetName());
		}
		EndAbility(this->CurrentSpecHandle, this->CurrentActorInfo, this->CurrentActivationInfo, true, true);
	}
}

void UCaptureAbility::InitCaptureFromPayload(const FGameplayEventData* Payload)
{
	this->Capturer = const_cast<AActor*>(Payload->Instigator);
	this->Captive = const_cast<AActor*>(Payload->Target);
	this->CapturerComponentToAttach = this->GetAttachComponent(this->Capturer, this->TagOfCapturerComponentToAttach);
	this->CaptiveComponentToAttach = this->GetAttachComponent(this->Captive, this->TagOfCaptiveComponentToAttach);

	if (this->bShouldDisableCaptiveCollision && IsValid(this->Captive))
	{
		this->Captive->SetActorEnableCollision(false);
	}

	this->NumStrugglesToEscape = (int)Payload->EventMagnitude;
	this->AddRequiredStrugglesToEscapeToCaptive(this->NumStrugglesToEscape);
	this->NumStrugglesAttempted = 0;
	this->CaptureStartTime = this->GetWorld()->GetTimeSeconds();
	if (IsValid(this->CapturerComponentToAttach))
	{
		FVector CapturerComponentLocation = this->CapturerComponentToAttach->GetComponentLocation();
		this->RelativeLocationAtStart = this->Captive->GetActorLocation() - CapturerComponentLocation;
	}

	UInputPressedAbilityTask* WaitInputPress = UInputPressedAbilityTask::InputPressed(this, this->StruggleInputID);
	WaitInputPress->OnPressed.AddUniqueDynamic(this, &UCaptureAbility::OnStrugglePressed);
	WaitInputPress->ReadyForActivation();

	UTickDurationAbilityTask* TickTask = UTickDurationAbilityTask::TickForDuration(this);
	TickTask->OnDurationTick.AddUniqueDynamic(this, &UCaptureAbility::OnCaptureTick);
	TickTask->ReadyForActivation();
}

USceneComponent* UCaptureAbility::GetAttachComponent(AActor* Actor, FString ComponentTag)
{
	USceneComponent* AttachComponent = nullptr;
	if (IsValid(Actor))
	{
		TArray<UActorComponent*> ComponentsWithAttachTag = Actor->GetComponentsByTag(USceneComponent::StaticClass(), FName(*ComponentTag));
		if (ComponentsWithAttachTag.Num() > 0)
		{
			AttachComponent = Cast<USceneComponent>(ComponentsWithAttachTag[0]);
		}
		else
		{
			AttachComponent = Actor->GetRootComponent();
		}
	}
	return AttachComponent;
}

void UCaptureAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if (this->bShouldDisableCaptiveCollision && IsValid(this->Captive))
	{
		this->Captive->SetActorEnableCollision(true);
	}

	TArray<UCaptureAbility*> AbilityInstances;
	int32 ActiveCaptures = UBaseGameplayAbility::GetActiveAbilityInstancesOfType(this->GetAbilitySystemComponentFromActorInfo(), AbilityInstances);
	if (ActiveCaptures == 1)
	{
		this->GetAbilitySystemComponentFromActorInfo()->ClearAbility(Handle);
		this->CaptureEnd(this->Captive);
	}

	Super::EndAbility(this->CurrentSpecHandle, this->CurrentActorInfo, this->CurrentActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UCaptureAbility::OnCaptureTick(float DeltaTime)
{
	// If the Capturer gets destroyed, cancel the ability.
	if (!IsValid(this->Capturer))
	{
		this->EndAbility(this->CurrentSpecHandle, this->CurrentActorInfo, this->CurrentActivationInfo, true, false);
	}

	if (IsValid(this->CapturerComponentToAttach))
	{
		FVector CapturerComponentLocation = this->CapturerComponentToAttach->GetComponentLocation();
		/*if (this->bIsAttachedByComponentTags && IsValid(this->CaptiveComponentToAttach))
		{
			FVector CaptiveComponentOffsetFromActor = this->CaptiveComponentToAttach->GetComponentLocation() - this->Captive->GetActorLocation();

			FVector CaptiveActorLocation = CapturerComponentLocation - CaptiveComponentOffsetFromActor;

			this->Captive->SetActorLocation(CaptiveActorLocation, false);
		}
		else if (this->bIsAttachedByRelativeLocationAtStart)
		{
			FVector CaptiveActorLocation = CapturerComponentLocation + this->RelativeLocationAtStart;
			this->Captive->SetActorLocation(CaptiveActorLocation, false);
		}*/
		if (IsValid(this->CaptiveComponentToAttach) && this->bIsAttachedByRelativeLocationAtStart)
		{
			FVector CaptiveComponentOffsetFromActor = this->CaptiveComponentToAttach->GetComponentLocation() - this->Captive->GetActorLocation();
			FVector CaptiveActorLocationFinal = CapturerComponentLocation - CaptiveComponentOffsetFromActor;

			FVector CaptiveActorLocationRelative = CapturerComponentLocation + this->RelativeLocationAtStart;

			this->SlideToLocationTimePassed += DeltaTime;
			float Alpha = FMath::Clamp(SlideToLocationTimePassed / SlideToLocationTimeFinished, 0.0f, 1.0f);
			FVector LerpedCaptiveActorLocation = FMath::Lerp(CaptiveActorLocationRelative, CaptiveActorLocationFinal, Alpha);

			this->Captive->SetActorLocation(LerpedCaptiveActorLocation, false);
		}
	}
}

void UCaptureAbility::OnStrugglePressed()
{
	if (!this->GetAbilitySystemComponentFromActorInfo()->HasAnyMatchingGameplayTags(this->IgnoreStrugglesWhenCaptiveHasTags))
	{
		this->NumStrugglesAttempted += 1;

		TArray<UCaptureAbility*> ActiveCaptures;
		UBaseGameplayAbility::GetActiveAbilityInstancesOfType(this->GetAbilitySystemComponentFromActorInfo(), ActiveCaptures);
		if (this == ActiveCaptures[0]) // If multiple captures are active, we only want 1 of them to decrement the struggle count.
		{
			this->RemoveRequiredStrugglesToEscapeToCaptive();
		}

		this->CheckStrugglesRemaining();
	}
}

void UCaptureAbility::CheckStrugglesRemaining()
{
	int32 RequiredStrugglesRemaining = this->GetRemainingRequiredStrugglesToEscapeCount();
	float TimeSinceCaptured = this->GetWorld()->GetTimeSeconds() - this->CaptureStartTime;

	TScriptInterface<ICapturerInterface> CapturerInterface = this->Capturer;
	if (RequiredStrugglesRemaining <= 0)
	{
		// Tell the capturer that the capturered actor tried to struggle, and it has escaped
		if (IsValid(this->Capturer) && CapturerInterface)
		{
			CapturerInterface->Execute_OnCapturedActorEscape(this->Capturer, this->NumStrugglesAttempted, TimeSinceCaptured);
		}
		EndAbility(this->CurrentSpecHandle, this->CurrentActorInfo, this->CurrentActivationInfo, true, false);
	}
	else
	{
		// Broadcast to our capturing actor that the capturered actor tried to struggle, but has not yet escaped
		if (IsValid(this->Capturer) && CapturerInterface)
		{
			CapturerInterface->Execute_OnCapturedActorStruggle(this->Capturer, this->NumStrugglesAttempted, RequiredStrugglesRemaining, TimeSinceCaptured);
		}
	}
}

void UCaptureAbility::AddRequiredStrugglesToEscapeToCaptive(int32 Count)
{
	UAbilitySystemComponent* ASC = this->GetAbilitySystemComponentFromActorInfo();
	for (int32 i = 0; i < Count; i++)
	{
		this->StrugglesToEscapeEffectHandle = ASC->ApplyGameplayEffectToSelf(this->StrugglesToEscapeEffect.GetDefaultObject(), 1, ASC->MakeEffectContext());
	}
}

void UCaptureAbility::RemoveRequiredStrugglesToEscapeToCaptive(int32 Count)
{
	this->GetAbilitySystemComponentFromActorInfo()->RemoveActiveGameplayEffect(this->StrugglesToEscapeEffectHandle, Count);
}

int32 UCaptureAbility::GetRemainingRequiredStrugglesToEscapeCount() const
{
	return this->GetAbilitySystemComponentFromActorInfo()->GetCurrentStackCount(this->StrugglesToEscapeEffectHandle);
}

UCaptureAbility* UCaptureAbility::GetAbilityInstanceForCapturer(const AActor* CapturerActor, const AActor* CaptiveActor)
{
	TArray<UCaptureAbility*> ActiveAbilities;
	TScriptInterface<IAbilitySystemInterface> ASCInterface = const_cast<AActor*>(CaptiveActor);
	if (ASCInterface)
	{
		UAbilitySystemComponent* ASC = ASCInterface->GetAbilitySystemComponent();
		UBaseGameplayAbility::GetActiveAbilityInstancesOfType(ASC, ActiveAbilities);

		for (UCaptureAbility* AbilityInstance : ActiveAbilities)
		{
			if (AbilityInstance->Capturer == CapturerActor)
			{
				return AbilityInstance;
			}
		}
	}
	return nullptr;
}
