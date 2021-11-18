// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "AbilitySystemInterface.h"
#include "Neokaiju/Ability/EnvironmentAbilities/CaptureAbility/CapturerInterface.h"
#include "GameplayTags.h"
#include "SwarmV2.generated.h"

class UAbilitySystemComponent;
class USwarmState;
class USwarmStats;
class AKaijuCharacter;
class USphereComponent;
class UStaticMeshComponent;
class UEnemySubsystem;

UCLASS()
class NEOKAIJU_API ASwarmV2 : public APawn, public IAbilitySystemInterface, public ICapturerInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ASwarmV2();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TArray<FRotator> Directions;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int32 NumDirs;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		USwarmState* CurrentState;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		AKaijuCharacter* TargetKaiju;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		USphereComponent* InnerRadius;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		USphereComponent* OuterRadius;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		UStaticMeshComponent* BulletMesh;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		UStaticMeshComponent* ClawMesh;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool ClawMode = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool bHitByLaser = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float MoveSpeed = 100.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float RotationSpeed = 100.0f;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
		FVector LastDirection;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		USwarmStats* Stats;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		UAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FGameplayTag HitByLaserTag;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FGameplayTag OverlapByLaserTag;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void KillSwarm();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void ListenForTags();

	UFUNCTION()
		void HitByLaserTagChanged(FGameplayTag Tag, int32 Count);

	UFUNCTION()
		void OverlapByLaserTagChanged(FGameplayTag Tag, int32 Count);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void BeginHitByLaser();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void EndHitByLaser();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void BeginOverlapByLaser();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void EndOverlapByLaser();

	UFUNCTION(BlueprintImplementableEvent)
		void Init();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
		void ChangeState(USwarmState* NewState);

	UFUNCTION(BlueprintCallable)
		void Move(float DeltaTime, float Speed, FVector Target);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		FVector FindOpenSpace(FVector Target, float MaxDist = 100.0f);
	virtual FVector FindOpenSpace_Implementation(FVector Target, float MaxDist);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		FVector FindOpenSpaceFrontBack(FVector Target, float MaxDist = 100.0f);
	virtual FVector FindOpenSpaceFrontBack_Implementation(FVector Target, float MaxDist);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void OrientSwarm(FRotator rot);
	virtual void OrientSwarm_Implementation(FRotator rot);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void RotateSwarm(float deltaTime);
	virtual void RotateSwarm_Implementation(float deltaTime);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void AssembleSwarm(float transformTime, float totalTransformTime);
	virtual void AssembleSwarm_Implementation(float transformTime, float totalTransformTime);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void DissembleSwarm(float transformTime, float totalTransformTime);
	virtual void DissembleSwarm_Implementation(float transformTime, float totalTransformTime);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void ScatterSwarmOut(float scatterTime, float totalScatterTime);
	virtual void ScatterSwarmOut_Implementation(float scatterTime, float totalScatterTime);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void ScatterSwarmIn(float scatterTime, float totalScatterTime);
	virtual void ScatterSwarmIn_Implementation(float scatterTime, float totalScatterTime);

	UFUNCTION(BlueprintCallable)
		void MakeDirections();

	UFUNCTION(BlueprintCallable)
		FVector FindTargetNearKaiju(float TargetDistance);

	UFUNCTION(BlueprintCallable)
		FVector FindStrafingTarget(bool ClockWise);

	UFUNCTION(BlueprintCallable)
		UEnemySubsystem* GetEnemySubsystem();

};

