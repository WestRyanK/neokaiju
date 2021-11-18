// Fill out your copyright notice in the Description page of Project Settings.





#include "KaijuCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Neokaiju/Ability/Core/AbilityInputID.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/Character.h"
#include "Components/InputComponent.h"
#include "Components/SceneComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "AbilitySystemInterface.h"
#include "Neokaiju/KaijuCharacter/KaijuAttributeSet.h"
#include "Neokaiju/KaijuCharacter/Components/AimComponent.h"
#include "Neokaiju/KaijuCharacter/Components/SocketSplineComponent.h"
#include "Neokaiju/KaijuCharacter/Components/EffectTriggerLineTraceComponent.h"
#include "Neokaiju/KaijuCharacter/Components/CharacterStateTaggerComponent.h"
#include "Neokaiju/KaijuCharacter/Components/TagAwareLerpedSpringArmComponent.h"
#include "Neokaiju/KaijuCharacter/Components/KaijuMovementComponent.h"
#include "Neokaiju/KaijuCharacter/Components/DebugCommandComponent.h"
#include "Neokaiju/KaijuCharacter/Components/CameraPanComponent.h"
#include "Neokaiju/KaijuCharacter/Components/KaijuCamera.h"
#include "Neokaiju/KaijuCharacter/KaijuCharacterEventSubsystem.h"
#include "Neokaiju/KaijuCharacter/KaijuCharacterSaveState.h"
#include "Neokaiju/Core/SaveGame/NeokaijuSaveGame.h"
#include "Neokaiju/Core/BaseCharacter.h"
#include "Neokaiju/Core/SafeDirectionUtility.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AKaijuCharacter::AKaijuCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	this->GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);

	// Setup Character movement
	this->bUseControllerRotationYaw = false;
	this->GetCharacterMovement()->AirControl = 0.7f;
	this->GetCharacterMovement()->AirControlBoostMultiplier = 0.0f;
	this->GetCharacterMovement()->AirControlBoostVelocityThreshold = 0.0f;

	this->CharacterRotationPoint = CreateDefaultSubobject<USceneComponent>(TEXT("CharacterRotationPoint"));
	this->CharacterRotationPoint->SetupAttachment(this->RootComponent);

	this->GetMesh()->SetupAttachment(this->CharacterRotationPoint);

	// Setup Aim Component
	this->AimComponent = CreateDefaultSubobject<UAimComponent>(TEXT("AimComponent"));
	this->AimComponent->DefaultAimForwardDirectionComponent = this->CharacterRotationPoint;

	this->AttachPosition = CreateDefaultSubobject<USceneComponent>(TEXT("AttachPosition"));
	this->AttachPosition->SetupAttachment(this->CharacterRotationPoint);
	this->TailHitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TailHitBox"));
	this->TailHitBox->SetupAttachment(this->CharacterRotationPoint);
	this->TailHitBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	this->PassivelyDestroyHitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("PassivelyDestroyHitBox"));
	this->PassivelyDestroyHitBox->SetupAttachment(this->CharacterRotationPoint);
	this->GroundDetectionHitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("GroundDetectionHitBox"));
	this->GroundDetectionHitBox->SetupAttachment(this->CharacterRotationPoint);

	// Setup Camera Focus point
	this->RotationScaleIndependentPoint = CreateDefaultSubobject<USceneComponent>(TEXT("RotationScaleIndependentPoint"));
	//this->RotationScaleIndependentPoint->SetUsingAbsoluteRotation(true);
	this->RotationScaleIndependentPoint->SetUsingAbsoluteScale(true);
	this->RotationScaleIndependentPoint->SetupAttachment(this->RootComponent);

	this->PerpendicularTagAwareSpringArm = CreateDefaultSubobject<UTagAwareLerpedSpringArmComponent>(TEXT("PerpendicularTagAwareSpringArm"));
	this->PerpendicularTagAwareSpringArm->SetupAttachment(this->RotationScaleIndependentPoint);

	this->CameraPanComponent = CreateDefaultSubobject<UCameraPanComponent>(TEXT("CameraPanComponent"));
	this->CameraPanComponent->SetupAttachment(this->PerpendicularTagAwareSpringArm, USpringArmComponent::SocketName);

	this->CameraFocusPoint = CreateDefaultSubobject<USceneComponent>(TEXT("CameraFocusPoint"));
	this->CameraFocusPoint->SetupAttachment(this->CameraPanComponent, UCameraPanComponent::SocketName);

	this->KaijuLevelSocketSpline = CreateDefaultSubobject<USocketSplineComponent>(TEXT("KaijuLevelSocketSpline"));
	this->KaijuLevelSocketSpline->SetupAttachment(this->CameraFocusPoint);

	// Setup Spring Arm that holds camera
	this->SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	this->SpringArm->SetupAttachment(this->KaijuLevelSocketSpline, USocketSplineComponent::SocketName);
	this->SpringArm->bUsePawnControlRotation = false;
	//this->SpringArm->SetUsingAbsoluteRotation(true);

	// A vertical spring arm to prevent clipping through ceilings and floors.
	this->VerticalSpringArm = CreateDefaultSubobject<USpringArmComponent>("VerticalSpringArm");
	this->VerticalSpringArm->SetupAttachment(this->SpringArm, USpringArmComponent::SocketName);
	this->VerticalSpringArm->bUsePawnControlRotation = false;

	// Setup Camera and put it in the Spring Arm
	this->KaijuCamera = CreateDefaultSubobject<UKaijuCamera>(TEXT("Camera"));
	this->KaijuCamera->SetupAttachment(this->VerticalSpringArm, USpringArmComponent::SocketName);
	this->KaijuCamera->bUsePawnControlRotation = false;

	// Setup TraceTrigger to detect if facing wall
	this->FacingWallTrigger = CreateDefaultSubobject<UEffectTriggerLineTraceComponent>(TEXT("FacingWallTrigger"));
	this->FacingWallTrigger->SetUsingAbsoluteRotation(true);
	this->FacingWallTrigger->SetupAttachment(this->RootComponent);

	this->CharacterStateTagger = CreateDefaultSubobject<UCharacterStateTaggerComponent>(TEXT("CharacterStateTagger"));
	this->DebugCommandHandler = CreateDefaultSubobject<UDebugCommandComponent>(TEXT("DebugCommandHandler"));

	// Since this was created in the Character's constructor, it is automatically registered with ASC.
	// https://github.com/tranek/GASDocumentation#441-attribute-set-definition
	this->AttributeSet = CreateDefaultSubobject<UKaijuAttributeSet>(TEXT("AttributeSet"));

	this->KaijuMovement = CreateDefaultSubobject<UKaijuMovementComponent>(TEXT("KaijuMovement"));

	this->ListenForAttributeChanges();
}

bool AKaijuCharacter::GetIsPlayerAiming()
{
	return this->AimComponent->bIsAimingPrimary;
}

FVector AKaijuCharacter::GetPrimaryAimDirection()
{
	return this->AimComponent->PrimaryDirection;
}

FVector AKaijuCharacter::GetSecondaryAimDirection()
{
	return this->AimComponent->SecondaryDirection;
}

void AKaijuCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	this->InputComponent->BindAxis("Right", this->KaijuMovement, &UKaijuMovementComponent::MoveKaijuCharacter);

	this->AbilitySystemComponent->BindAbilityActivationToInputComponent(
		PlayerInputComponent,
		FGameplayAbilityInputBinds(FString("ConfirmTarget"),
			FString("CancelTarget"),
			FString("EAbilityInputID"),
			static_cast<int32>(EAbilityInputID::Confirm),
			static_cast<int32>(EAbilityInputID::Cancel)));
}


void AKaijuCharacter::BeginPlay()
{
	// We don't want to clamp while we're applying initial effects (which is what our Super::BeginPlay does).
	// We don't want to clamp because if health is set before maxhealth then it will be clamped wrong.
	this->AttributeSet->bShouldClampAttributes = false;
	Super::BeginPlay();
	this->AttributeSet->bShouldClampAttributes = true;
}

void AKaijuCharacter::ListenForAttributeChanges()
{
	this->AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(this->AttributeSet->GetHealthAttribute()).AddUObject(this, &AKaijuCharacter::HealthChanged);
	this->AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(this->AttributeSet->GetMaxHealthAttribute()).AddUObject(this, &AKaijuCharacter::HealthChanged);
	this->AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(this->AttributeSet->GetKaijuSizeScaleAttribute()).AddUObject(this, &AKaijuCharacter::KaijuSizeScaleChanged);
	this->AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(this->AttributeSet->GetKaijuLevelAttribute()).AddUObject(this, &AKaijuCharacter::KaijuLevelChanged);
	this->AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(this->AttributeSet->GetLaserChargeAttribute()).AddUObject(this, &AKaijuCharacter::LaserChargeChanged);
	this->AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(this->AttributeSet->GetMaxLaserChargeAttribute()).AddUObject(this, &AKaijuCharacter::LaserChargeChanged);
}

void AKaijuCharacter::HealthChanged(const FOnAttributeChangeData& Data)
{
	UKaijuCharacterEventSubsystem* KaijuCharacterEventSubsystem = this->GetWorld()->GetSubsystem<UKaijuCharacterEventSubsystem>();
	float NewHealth = this->GetHealth();
	float OldHealth = NewHealth;

	if (Data.Attribute == UKaijuAttributeSet::GetHealthAttribute())
	{
		OldHealth = Data.OldValue;
	}

	KaijuCharacterEventSubsystem->KaijuHealthChanged(OldHealth, NewHealth, this->GetMaxHealth());

	if (!this->bIsLoadingState)
	{
		if (NewHealth < OldHealth)
		{
			this->OnDamageTaken(OldHealth, NewHealth);
		}
	}
}

void AKaijuCharacter::KaijuLevelChanged(const FOnAttributeChangeData& Data)
{
	// Update the position of the camera on the level spline.
	this->KaijuLevelSocketSpline->SetSplineSocketPositionKeyDelayed(Data.NewValue);
	this->OnKaijuLevelChanged(Data.OldValue, Data.NewValue);
}

void AKaijuCharacter::KaijuSizeScaleChanged(const FOnAttributeChangeData& Data)
{
	//this->SetActorScale3D(FVector(Data.NewValue));
}

void AKaijuCharacter::LaserChargeChanged(const FOnAttributeChangeData& Data)
{
	float NewCharge = this->GetLaserCharge();
	float OldCharge = NewCharge;

	if (Data.Attribute == UKaijuAttributeSet::GetLaserChargeAttribute())
	{
		OldCharge = Data.OldValue;
	}

	UKaijuCharacterEventSubsystem* KaijuCharacterEventSubsystem = this->GetWorld()->GetSubsystem<UKaijuCharacterEventSubsystem>();
	KaijuCharacterEventSubsystem->KaijuLaserChargeChanged(OldCharge, NewCharge, this->GetMaxLaserCharge());
}

// Begin ISaveableInterface
void AKaijuCharacter::BeginLoadState()
{
	this->bIsLoadingState = true;
}

void AKaijuCharacter::EndLoadState()
{
	this->bIsLoadingState = false;
}
// End ISaveableInterface

float AKaijuCharacter::GetScaledCollisionDepthDistance() const
{
	float KaijuSizeScale = this->AbilitySystemComponent->GetNumericAttribute(UKaijuAttributeSet::GetKaijuSizeScaleAttribute());
	return this->CollisionDepthDistance * KaijuSizeScale;
}

void AKaijuCharacter::OnConstruction(const FTransform& Transform)
{
	this->SetBoxDepth(this->TailHitBox, this->CollisionDepthDistance);
	this->SetBoxDepth(this->PassivelyDestroyHitBox, this->CollisionDepthDistance);
	this->SetBoxDepth(this->GroundDetectionHitBox, this->CollisionDepthDistance);
	this->FacingWallTrigger->CollisionDepthDistance = this->CollisionDepthDistance;
}

void AKaijuCharacter::SetBoxDepth(UBoxComponent* BoxComponent, float Depth)
{
	if (IsValid(BoxComponent))
	{
		FVector Extent = BoxComponent->GetUnscaledBoxExtent();
		Extent.Y = Depth;
		BoxComponent->SetBoxExtent(Extent);
	}
}
