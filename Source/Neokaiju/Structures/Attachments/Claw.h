// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Neokaiju/Structures/Attachments/Attachment.h"
#include "MoveClawComponent.h"
#include "Neokaiju/Ability/EnvironmentAbilities/CaptureAbility/CaptureAbility.h"
#include "Neokaiju/Ability/EnvironmentAbilities/CaptureAbility/CapturerInterface.h"
#include "Claw.generated.h"

class AActor;
class AKaijuCharacter;
class ABuildingBlock;


UCLASS()
class NEOKAIJU_API AClaw : public AAttachment, public ICapturerInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AClaw();

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Component")
		class UBoxComponent* ActiveArea;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Component")
		class UBoxComponent* KaijuTargetArea;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Component")
		class UStaticMeshComponent* CraneBoxComponent;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Component")
		class USkeletalMeshComponent* ClawMesh;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Component")
		class UBoxComponent* CheckLeftHitBox;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Component")
		class UBoxComponent* CheckRightHitBox;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Component")
		class UBoxComponent* ClawHitBox;

	// Variables
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Claw", Meta = (ClampMin = "0", ClampMax = "5", DisplayPriority = "1"))
		int CraneBoxTier;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Claw", Meta = (DisplayAfter = "CraneBoxTier"))
		float CraneBoxSpeed = 300.0f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Claw", Meta = (DisplayAfter = "CraneBoxSpeed"))
		float ClawLoweringSpeed = 400.0f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Claw", Meta = (DisplayAfter = "ClawLoweringSpeed"))
		float ClawRaisingSpeed = 200.0f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Claw", Meta = (DisplayAfter = "ClawRaisingSpeed"))
		float TimeToCatch = 0.5f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Claw", Meta = (DisplayAfter = "TimeToCatch"))
		int NumStrugglesToEscapeCapture = 3;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Claw", Meta = (DisplayAfter = "NumStrugglesToEscapeCapture"))
		class UCurveFloat* ClawMovementCurve;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Claw", Meta = (DisplayAfter = "ClawMovementCurve"))
		class UStaticMesh* ClawStaticMesh;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Claw", Meta = (DisplayAfter = "ClawStaticMesh"))
		bool MoveBlocks = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Claw", Meta = (DisplayAfter = "MoveBlocks"))
		TArray<ABuildingBlock*> MovableBlocks;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
		class TSubclassOf<class UCaptureAbility> CaptureAbility;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void AttachedBlockUnstable(ABreakableStructure* UnstableStructure) override;
	virtual void AttachedBlockDestroyed(AActor* DestroyedActor) override;
	void DestroyClaw();

	UFUNCTION(BlueprintCallable)
		void OnActiveAreaBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION(BlueprintCallable)
		void OnActiveAreaEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable)
		void OnKaijuTargetAreaBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION(BlueprintCallable)
		void OnKaijuTargetAreaEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable)
		void OnClawHitBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
		void OnClawReachedEnd();

	UFUNCTION(BlueprintCallable)
		void StartPauseForAnimations();
	UFUNCTION(BlueprintCallable)
		void StopPauseForAnimations();

	UFUNCTION(BlueprintCallable)
		void Track(float DeltaTime);

	// Moves crane box towards a location and returns true if it is within the 'close enough' distance.
	// isAtEndOfRail is an out variable
	UFUNCTION(BlueprintCallable)
		bool MoveCraneBoxTowardsLocation(FVector Location, float Speed, float DeltaTime, bool& isAtEndOfRail);

	UFUNCTION(BlueprintCallable)
		void CarryObjective(float DeltaTime);



	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void OnCapturedActorStruggle(int StruggleAttemptNum, int StrugglesRemainingToEscape, float TimeSinceCaptureStarted);
	virtual void OnCapturedActorStruggle_Implementation(int StruggleAttemptNum, int StrugglesRemainingToEscape, float TimeSinceCaptureStarted);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void OnCapturedActorEscape(int StruggleAttemptNum, float TimeSinceCaptureStarted);
	virtual void OnCapturedActorEscape_Implementation(int StruggleAttemptNum, float TimeSinceCaptureStarted);

	// Blueprint events
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Claw")
		void PlayOnReadyFX();
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Claw")
		void PlayOnCaptureFX();
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Claw")
		void PlayOnReleaseFX();
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Claw")
		void PlayOnCharacterStruggleFX();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Claw")
		void PlayOnClawExtendFX();
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Claw")
		void PlayOnClawMoveFX();
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Claw")
		void StopOnClawMoveFX();

private:
	// Variables
	class ABuildingBlock* BB_CraneBox;
	AKaijuCharacter* Character;
	AActor* CurrentObjective;
	FVector LocationObjective;
	UMoveClawComponent* MoveClawComponent;

	float LeftLimit = 0;
	float RightLimit = 0;
	float LowerLimit = 0;
	float InitialClawHeight;

	float CloseEnough = 30.0f;
	float Timer = 0.0f;

	// Get different block each time we start a new block objective
	int BlockIterator = 0;


	// Claw States
	bool Active = false; // We are active while character is inside our active range (box)
	bool KaijuInTarget = false; // True when character is inside the kaiju target area
	bool Tracking = false;
	bool Lowering = false;
	bool Raising = false;
	bool CaughtCharacter = false;
	bool CaughtBlock = false;

	bool PausedForAnimation = false;


	// FUNCTIONS
	UFUNCTION(Category = "Claw")
		void OnCraneBoxDestroyed(AActor* DestroyedCraneBox);
	
	// Nullifies current objective. Then will make character the objective if character is in our area. Otherwise start a new block objective.
	void StartNewObjective();

	void SetCharacterObjective();
	void SetNewBlockObjective();

	void FindPlaceToDropBox();
	void DropBlock();

	void InitActiveLimits();
	void CheckAttachedBlock();

	// Block is an out parameter
	bool HasBlock(class UPrimitiveComponent* Component, ABuildingBlock* Block);
};
