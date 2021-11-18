// Fill out your copyright notice in the Description page of Project Settings.


#include "GrabAbility.h"
#include "Neokaiju/KaijuCharacter/KaijuCharacter.h"
#include "Neokaiju/Ability/Core/Tasks/TickDurationAbilityTask.h"
#include "DrawDebugHelpers.h"
#include "Neokaiju/Structures/Grabbable.h"
#include "Neokaiju/Structures/BreakableStructures/BreakableStructure.h"
#include "Neokaiju/Structures/BreakableStructures/BreakableStructureSubsystem.h"
#include "Neokaiju/Structures/BreakableStructures/BuildingBlocks/BlockManager.h"
#include "Neokaiju/Structures/BreakableStructures/BuildingBlocks/BuildingBlock.h"
#include "Neokaiju/Structures/BreakableStructures/BuildingBlocks/SpikeBlock.h"
#include "Neokaiju/Structures/BreakableStructures/BuildingBlocks/LaserOnlyBlock.h"
#include "Neokaiju/Ability/Core/Tasks/RepeatInfiniteAbilityTask.h"
#include "Neokaiju/KaijuCharacter/Components/KaijuMovementComponent.h"
#include "Neokaiju/KaijuCharacter/Components/AimComponent.h"
#include "Engine/Engine.h"
#include "Neokaiju/Core/TraceUtility.h"
#include "Neokaiju/Ability/Core/Tasks/InputReleaseOrDelayAbilityTask.h"
#include "Abilities/Tasks/AbilityTask_WaitInputRelease.h"
#include "Abilities/Tasks/AbilityTask_WaitInputPress.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayTag.h"
#include "Abilities/Tasks/AbilityTask_WaitDelay.h"
#include "Components/BoxComponent.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "Kismet/GameplayStatics.h"

UGrabAbility::UGrabAbility()
{
	this->InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

AActor* UGrabAbility::TraceForward()
{
	FVector Start = this->Character->GetMesh()->GetSocketLocation(this->MouthSocketName);

	FVector GrabDirection;
	if (this->Character->GetIsPlayerAiming())
	{
		GrabDirection = this->Character->GetSecondaryAimDirection();
	}
	else
	{
		GrabDirection = FVector::DownVector + this->Character->KaijuMovement->GetCharacterForward();
	}

	FVector End = Start + GrabDirection.GetSafeNormal() * GrabRange * this->Character->GetKaijuSizeScale();

	this->GrabWidths.Sort();

	FHitResult Hit;
	bool bDidHitOccur = false;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this->Character);

	for (float GrabWidth : this->GrabWidths)
	{
		bDidHitOccur = UTraceUtility::BoxTrace(
			this,
			Hit,
			Start,
			End,
			GrabWidth * this->Character->GetKaijuSizeScale(),
			this->Character->GetScaledCollisionDepthDistance(),
			ECC_Visibility,
			QueryParams,
			this->bIsAbilityDebugEnabled, 0.25f);

		if (bDidHitOccur)
		{
			AActor* HitActor = Hit.GetActor();
			if (IsValid(HitActor))
			{
				IGrabbable* Grabbable = Cast<IGrabbable>(HitActor);
				if (Grabbable)
				{
					GrabLocation = Hit.Location;
					return HitActor;
				}
			}
		}
	}

	return nullptr;
}

TSet<AActor*> UGrabAbility::TraceDown()
{
	TSet<AActor*> OverlappingActors;
	this->Character->GroundDetectionHitBox->GetOverlappingActors(OverlappingActors, ABuildingBlock::StaticClass());

	return OverlappingActors;
}

bool UGrabAbility::IsKaijuStandingOnConnectedBlock(ABuildingBlock* GrabbedBlock)
{
	// check if the block you're trying to grab will cause the structure you're standing on to move
	UBreakableStructureSubsystem* BreakableStructureSubsystem = this->GetWorld()->GetSubsystem<UBreakableStructureSubsystem>();
	TSet<AActor*> ActorUnderFeet = TraceDown();
	bool IsConnectedToBlockStoodOn = false;
	for (AActor* Block : ActorUnderFeet)
	{
		if (IsValid(Block))
		{
			ABuildingBlock* BlockUnderFeet = Cast<ABuildingBlock>(Block);
			if (IsValid(BlockUnderFeet) && IsValid(GrabbedBlock))
			{
				if (!BlockUnderFeet->GetName().Equals(GrabbedBlock->GetName()))
				{

					IsConnectedToBlockStoodOn = BreakableStructureSubsystem->BlockManager->BlocksAttached(BlockUnderFeet, GrabbedBlock);
				}

			}
		}
		if (IsConnectedToBlockStoodOn)
		{
			break;
		}
	}

	return IsConnectedToBlockStoodOn;
}

bool UGrabAbility::IsKaijuStandingOnGrabbedBlock(ABuildingBlock* GrabbedBlock)
{
	// check if the block you're trying to grab is the one you're standing on
	UBreakableStructureSubsystem* BreakableStructureSubsystem = this->GetWorld()->GetSubsystem<UBreakableStructureSubsystem>();
	TSet<AActor*> ActorUnderFeet = TraceDown();
	bool IsBlockStoodOn = false;
	for (AActor* Block : ActorUnderFeet)
	{
		if (IsValid(Block))
		{
			ABuildingBlock* BlockUnderFeet = Cast<ABuildingBlock>(Block);
			if (IsValid(BlockUnderFeet) && IsValid(GrabbedBlock))
			{
				if (BlockUnderFeet->GetName().Equals(GrabbedBlock->GetName()))
				{

					IsBlockStoodOn = true;
				}

			}
		}
		if (IsBlockStoodOn)
		{
			break;
		}
	}

	return IsBlockStoodOn;
}

bool UGrabAbility::CanKaijuGrab(AActor* GrabbedActor)
{
	IGrabbable* Grabbable = Cast<IGrabbable>(GrabbedActor);
	ABreakableStructure* BreakableStructure = Cast<ABreakableStructure>(GrabbedActor);
	ABuildingBlock* BuildingBlock = Cast<ABuildingBlock>(GrabbedActor);
	if (IsValid(GrabbedActor) && Grabbable && IsValid(BreakableStructure) && BuildingBlock && !IsKaijuStandingOnGrabbedBlock(BuildingBlock))
	{
		ASpikeBlock* SpikeBlock = Cast<ASpikeBlock>(BuildingBlock);
		ALaserOnlyBlock * LaserOnlyBlock = Cast<ALaserOnlyBlock>(BuildingBlock);
		if (IsValid(SpikeBlock) || IsValid(LaserOnlyBlock))
		{
			return false;
		}

		float KaijuLevel = this->Character->GetAbilitySystemComponent()->GetNumericAttribute(UKaijuAttributeSet::GetKaijuLevelAttribute());
		float MinimumKaijuLevelToGrab = BreakableStructure->GetAbilitySystemComponent()->GetNumericAttribute(UBreakableStructureAttributeSet::GetMinimumKaijuLevelToGrabAttribute());
		bool bIsKaijuHighEnoughLevelToGrab = (KaijuLevel >= MinimumKaijuLevelToGrab);

		return bIsKaijuHighEnoughLevelToGrab;
	}
	else
	{
		return false;
	}
}

void UGrabAbility::StartAbilityTasks()
{
	URepeatInfiniteAbilityTask* CheckGrabbedExistsTask = URepeatInfiniteAbilityTask::RepeatAction(this, this->TimeBetweenGrabbedExistsChecks);
	CheckGrabbedExistsTask->OnPerformAction.AddUniqueDynamic(this, &UGrabAbility::CheckGrabbedExists);
	CheckGrabbedExistsTask->ReadyForActivation();

	//UAbilityTask_WaitGameplayTagAdded* DropTask = UAbilityTask_WaitGameplayTagAdded::WaitGameplayTagAdd(this, FGameplayTag::RequestGameplayTag(FName("State.Falling")), nullptr, true);
	//DropTask->Added.AddDynamic(this, &UGrabAbility::DropObject);
	//DropTask->ReadyForActivation();

	if (this->bIsReleaseInputToThrow)
	{
		UAbilityTask_WaitInputRelease* WaitThrowTask = UAbilityTask_WaitInputRelease::WaitInputRelease(this, true);
		WaitThrowTask->OnRelease.AddDynamic(this, &UGrabAbility::ThrowObject);
		WaitThrowTask->ReadyForActivation();
	}
	else
	{
		UAbilityTask_WaitInputPress* WaitThrowTask = UAbilityTask_WaitInputPress::WaitInputPress(this);
		WaitThrowTask->OnPress.AddDynamic(this, &UGrabAbility::ThrowObject);
		WaitThrowTask->ReadyForActivation();
	}
}

bool UGrabAbility::CanKaijuGrabBlockGroup(ABuildingBlock* GrabbedBlock)
{
	UBreakableStructureSubsystem* BreakableStructureSubsystem = this->GetWorld()->GetSubsystem<UBreakableStructureSubsystem>();
	float KaijuLevel = this->Character->GetAbilitySystemComponent()->GetNumericAttribute(UKaijuAttributeSet::GetKaijuLevelAttribute());
	bool bIsKaijuLevelHigherThanGroupLevel = (KaijuLevel >= BreakableStructureSubsystem->BlockManager->MinimumLevelToGrabGroup(GrabbedBlock->Neighbors));
	bool bCanGrabGroup = !this->IsKaijuStandingOnConnectedBlock(GrabbedBlock) && bIsKaijuLevelHigherThanGroupLevel;
	return bCanGrabGroup;
}

void UGrabAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!this->CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		this->EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);


	UInputReleaseOrDelayAbilityTask* ReachDelayTask = UInputReleaseOrDelayAbilityTask::WaitInputReleaseOrDelay(this, this->GrabDelay);
	ReachDelayTask->OnComplete.AddUniqueDynamic(this, &UGrabAbility::ReachComplete);
	ReachDelayTask->OnCancel.AddUniqueDynamic(this, &UGrabAbility::ReachCancel);
	ReachDelayTask->ReadyForActivation();

	// There is a bug if you repeatedly grab quickly when using the delay. So commenting it out for now - RyanW

	//UAbilityTask_WaitDelay* AnimationDelayTask = UAbilityTask_WaitDelay::WaitDelay(this, this->GrabDelay);
	//AnimationDelayTask->OnFinish.AddUniqueDynamic(this, &UGrabAbility::ActivateGrab);
	//AnimationDelayTask->ReadyForActivation();
	//ActivateGrab();
}

void UGrabAbility::ReachCancel(float TimeRemaining)
{
	/*ReachComplete(TimeRemaining);
	if (IsValid(this->GrabbedActorRef))
	{
		ThrowObject(0);
	}*/
	this->EndAbility(this->CurrentSpecHandle, this->CurrentActorInfo, this->CurrentActivationInfo, true, true);
}

void UGrabAbility::ReachComplete(float TimeRemaining)
{
	this->Character = Cast<AKaijuCharacter>(this->GetAvatarActorFromActorInfo());
	if (IsValid(this->Character))
	{
		AActor* GrabbedActor = TraceForward();
		if (this->CanKaijuGrab(GrabbedActor))
		{
			this->GrabbedActorRef = GrabbedActor;
			ABuildingBlock* GrabbedBlock = Cast<ABuildingBlock>(GrabbedActor);
			bool bCanGrabGroup = this->CanKaijuGrabBlockGroup(GrabbedBlock);
			GrabbedBlock->Execute_Grab(this->GrabbedActorRef, this->Character->GetMesh(), this->MouthSocketName, GrabLocation, bCanGrabGroup);
			this->OnGrab();

			this->StartAbilityTasks();

			if (IsValid(this->OwnerEffectWhileGrabbing))
			{
				this->OwnerEffectWhileGrabbingHandle = this->GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectToSelf(this->OwnerEffectWhileGrabbing.GetDefaultObject(), 1, this->GetAbilitySystemComponentFromActorInfo()->MakeEffectContext());
			}

			// Create indicator spline.
			SplineRef = NewObject<USplineComponent>(Character, TEXT("AimSpline"));
			if (SplineRef)
			{
				SplineRef->RegisterComponent();
				SplineRef->AttachTo(Character->GetRootComponent(), NAME_None);
			}


			UTickDurationAbilityTask* Task = UTickDurationAbilityTask::TickForDuration(this);
			Task->OnDurationTick.AddUniqueDynamic(this, &UGrabAbility::Tick);
			Task->ReadyForActivation();
		}
		else
		{
			this->EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		}
	}
	else
	{
		this->EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
	}
}

void UGrabAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if (OwnerEffectWhileGrabbingHandle.IsValid())
	{
		this->GetAbilitySystemComponentFromActorInfo()->RemoveActiveGameplayEffect(this->OwnerEffectWhileGrabbingHandle);
	}

	if (IsValid(this->GrabbedActorRef)) 
	{
		this->DropObject();
	}

	// remove spline
	if (IsValid(this->SplineRef))
	{
		this->SplineRef->DestroyComponent();
	}

	if (IsValid(this->Character))
	{
		// make spline meshes invisible
		for (USplineMeshComponent* Mesh : this->Character->SplineMeshes)
		{
			Mesh->SetVisibility(false, false);
		}
	}

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

bool UGrabAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}

	return true;
}

bool UGrabAbility::CommitAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	if (!Super::CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		return false;
	}

	return true;
}

FVector UGrabAbility::GetAdjustedThrowAim()
{
	FVector ForwardVector = Character->CharacterRotationPoint->GetForwardVector();
	FVector AimVector = Character->GetSecondaryAimDirection();

	/* This function disables the ability to throw backwards
	if ((ForwardVector.X > 0 && AimVector.X < 0) || (ForwardVector.X < 0 && AimVector.X > 0)) {
		AimVector.Z += abs(AimVector.X);
		AimVector.X = 0;
	}
	*/

	// Throw a few degrees higher than the aim direction because if we throw straight to the side it won't be a good throw.
	FVector AdjustedAimVector = AimVector;
	if (!Character->AimComponent->bIsAimingSecondary && this->RotateAimDirectionUpByDegrees > 0.0f)
	{
		AdjustedAimVector = USafeDirectionUtility::RotateVectorOnYAxisTowards(AimVector, FVector::UpVector, ForwardVector, FVector::ZeroVector, this->RotateAimDirectionUpByDegrees, 1.0f);
	}
	return AdjustedAimVector;
}

void UGrabAbility::ThrowObject(float TimeHeld)
{
	if (IsValid(this->Character))
	{
		if (IsValid(GrabbedActorRef))
		{
			IGrabbable* Grabbable = Cast<IGrabbable>(GrabbedActorRef);
			if (Grabbable)
			{
				FVector AdjustedAimVector = this->GetAdjustedThrowAim();
				AimDirection = AdjustedAimVector; //Added by John so that I can use to determine throwing up or down

				Grabbable->Execute_Throw(GrabbedActorRef, AdjustedAimVector, ThrowVelocityMultiplier, this->ThrownObjectEffect);


				IAbilitySystemInterface* ASCInterface = Cast<IAbilitySystemInterface>(GrabbedActorRef);
				if (ASCInterface && IsValid(this->ThrownObjectEffect))
				{
					UAbilitySystemComponent* ThrownASC = ASCInterface->GetAbilitySystemComponent();

					ThrownASC->ApplyGameplayEffectToSelf(this->ThrownObjectEffect.GetDefaultObject(), 1, ThrownASC->MakeEffectContext());
				}
			}
		}
	}

	GrabbedActorRef = nullptr;
	this->EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);

	this->OnThrow();
}

void UGrabAbility::DropObject()
{
	if (IsValid(this->Character))
	{
		if (IsValid(GrabbedActorRef))
		{
			IGrabbable* Grabbable = Cast<IGrabbable>(GrabbedActorRef);
			if (Grabbable)
			{
				FVector ForwardVector = this->Character->CharacterRotationPoint->GetForwardVector();
				Grabbable->Execute_Throw(GrabbedActorRef, ForwardVector, 0, nullptr);
			}
		}
	}

	GrabbedActorRef = nullptr;
	this->EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
void UGrabAbility::CheckGrabbedExists(int32 CheckNumber)
{
	if (!IsValid(this->GrabbedActorRef))
	{
		this->EndAbility(this->CurrentSpecHandle, this->CurrentActorInfo, this->CurrentActivationInfo, true, true);
	}
}

void UGrabAbility::PredictProjectilePath()
{
	if (!IsValid(this->GrabbedActorRef))
	{
		UE_LOG(LogTemp, Display, TEXT("Grab: GrabbedActorRef is null"));
		return;
	}

	FPredictProjectilePathParams PathParams;
	PathParams.LaunchVelocity = this->GetAdjustedThrowAim() * this->ThrowVelocityMultiplier;
	PathParams.ActorsToIgnore.Add(this->Character);
	PathParams.ActorsToIgnore.Add(this->GrabbedActorRef);
	// override gravity?
	PathParams.StartLocation = this->GrabbedActorRef->GetActorLocation();
	PathParams.bTraceComplex = true; //?
	PathParams.SimFrequency = this->ThrowArcFrequency; //?
	PathParams.bTraceWithCollision = true; //?
	PathParams.MaxSimTime = this->ThrowArcSimTime;
	FPredictProjectilePathResult PathResult;
	UGameplayStatics::PredictProjectilePath(GrabbedActorRef, PathParams, PathResult);

	// TODO make a spline mesh
	if (SplineRef)
	{
		SplineRef->ClearSplinePoints();
		//SplineRef->SetWorldLocation(PathResult.PathData[0].Location);

		int Len = PathResult.PathData.Num();
		for (int i = 0; i < Len; i++)
		{
			SplineRef->AddSplinePoint(PathResult.PathData[i].Location, ESplineCoordinateSpace::World);
		}

		this->Character->UpdateSplineMeshes(SplineRef);

		// OLD
		/*for (int i = 0; i < Len - 1; i++)
		{
			FVector Start = PathResult.PathData[i].Location;
			FVector End = PathResult.PathData[i + 1].Location;

			DrawDebugLine(this->GetWorld(), Start, End, FColor::Cyan, false, 0.0f, 0, ThrowArcThickness);
		}*/
	}
}

void UGrabAbility::Tick(float DeltaTime)
{
	UE_LOG(LogTemp, Display, TEXT("grabbing..."));
	PredictProjectilePath();
}