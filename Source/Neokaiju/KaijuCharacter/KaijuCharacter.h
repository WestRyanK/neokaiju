// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Neokaiju/Core/BaseCharacter.h"
#include "Neokaiju/Core/SafeDirectionUtility.h"
#include "Neokaiju/KaijuCharacter/KaijuAttributeSet.h"
#include "Neokaiju/Core/SaveGame/SaveableInterface.h"
#include "KaijuCharacter.generated.h"

class USceneComponent;
class UBoxComponent;
class USpringArmComponent;
class USplineComponent;
class USplineMeshComponent;
class UCameraComponent;
class UCameraPanComponent;
class IAbilitySystemInterface;
class UAimComponent;
class UKaijuCamera;
class USocketSplineComponent;
class UEffectTriggerLineTraceComponent;
class UCharacterStateTaggerComponent;
class UTagAwareLerpedSpringArmComponent;
class UDebugCommandComponent;
class UKaijuMovementComponent;



/**
*  Represents the main character of Neokaiju. We're trying to encapsulate most functionality of the character in GameplayAbilities and Components, that way the character doesn't become cluttered.
*/
UCLASS()
class NEOKAIJU_API AKaijuCharacter : public ABaseCharacter, public ISaveableInterface
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		class UCharacterStateTaggerComponent* CharacterStateTagger;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		class UDebugCommandComponent* DebugCommandHandler;

	UPROPERTY()
		class UKaijuAttributeSet* AttributeSet;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		class UKaijuMovementComponent* KaijuMovement;

	// Represents the position that the camera is focusing. Move this to move where the camera is centered.
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		class USceneComponent* CameraFocusPoint;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		class UCameraPanComponent* CameraPanComponent;

	// The CameraFocusPoint should not rotate with the Character, so it is attached to this Component which maintains the same WorldRotation independent of Character.
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		class USceneComponent* RotationScaleIndependentPoint;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		class USceneComponent* CharacterRotationPoint;;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		class UKaijuCamera* KaijuCamera;

	// A spline which allows the designer to specify where the camera should be placed for each of the KaijuCharacter's levels.
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		class USocketSplineComponent* KaijuLevelSocketSpline;

	// Moves camera left and right to prevent geometry to the right of the character from obstructing the view.
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		class UTagAwareLerpedSpringArmComponent* PerpendicularTagAwareSpringArm;

	// Keeps camera movement smooth and prevents camera from intersecting the scene geometry.
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		class USpringArmComponent* SpringArm;

	// A vertical spring arm to prevent clipping through ceilings and floors.
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		class USpringArmComponent* VerticalSpringArm;

	// A component which captures the direction the player is aiming (with keyboard or joystick).
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		class UAimComponent* AimComponent;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		class USceneComponent* AttachPosition;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		class UBoxComponent* TailHitBox;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		class UBoxComponent* PassivelyDestroyHitBox;

	// Used by the grab ability to see what block are stood on
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		class UBoxComponent* GroundDetectionHitBox;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		class UEffectTriggerLineTraceComponent* FacingWallTrigger;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		TArray <USplineMeshComponent*> SplineMeshes;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		int SplineCount = 10;

public:
	// Sets default values for this character's properties
	AKaijuCharacter();

	virtual void OnConstruction(const FTransform& Transform) override;

	ATTRIBUTESET_PROPERTY_GETTER(KaijuLevel);
	ATTRIBUTESET_PROPERTY_GETTER(Health);
	ATTRIBUTESET_PROPERTY_GETTER(MaxHealth);
	ATTRIBUTESET_PROPERTY_GETTER(LaserDamage);
	ATTRIBUTESET_PROPERTY_GETTER(LaserCharge);
	ATTRIBUTESET_PROPERTY_GETTER(MaxLaserCharge);
	ATTRIBUTESET_PROPERTY_GETTER(DashDamage);
	ATTRIBUTESET_PROPERTY_GETTER(DashDistance);
	ATTRIBUTESET_PROPERTY_GETTER(DashDuration);
	ATTRIBUTESET_PROPERTY_GETTER(JumpHeight);
	ATTRIBUTESET_PROPERTY_GETTER(WalkSpeed);
	ATTRIBUTESET_PROPERTY_GETTER(KaijuSizeScale);
	ATTRIBUTESET_PROPERTY_GETTER(LaserLength);

	bool GetIsPlayerAiming();

	FVector GetPrimaryAimDirection();
	FVector GetSecondaryAimDirection();

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// The distance that ability's collisions extend into the foreground and background.
	// This value is based on when the kaiju is level 1 size.
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		float CollisionDepthDistance = 40;

	// When the Kaiju levels up, how long of a delay should there be (in seconds)
	// before the camera moves to its new location?
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		float KaijuLevelCameraPositionDelaySeconds = 2.5f;

	// Gets the CollisionDepthDistance scaled by the size of the kaiju (which is based on its level).
	UFUNCTION(BlueprintCallable, BlueprintPure)
		float GetScaledCollisionDepthDistance() const;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void OnDamageTaken(float OldHealth, float NewHealth);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void OnKaijuLevelChanged(float OldLevel, float NewLevel);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void UpdateSplineMeshes(USplineComponent* SplineRef);

	// ISaveableInterface Methods
	virtual void BeginLoadState();

	virtual void EndLoadState();

protected:
	UPROPERTY(BlueprintReadOnly)
		bool bIsLoadingState;

	virtual void BeginPlay() override;

	virtual void HealthChanged(const FOnAttributeChangeData& Data);

	virtual void KaijuLevelChanged(const FOnAttributeChangeData& Data);

	virtual void KaijuSizeScaleChanged(const FOnAttributeChangeData& Data);

	virtual void LaserChargeChanged(const FOnAttributeChangeData& Data);

	void ListenForAttributeChanges();

	void SetBoxDepth(UBoxComponent* BoxComponent, float Depth);
};
