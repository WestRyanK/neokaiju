// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SplineComponent.h"
#include "TimerManager.h"
#include "SocketSplineComponent.generated.h"

class ULerpUtility;
/**
 *
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class NEOKAIJU_API USocketSplineComponent : public USplineComponent
{
	GENERATED_BODY()

public:
	USocketSplineComponent();

	virtual bool HasAnySockets() const override;
	virtual FTransform GetSocketTransform(FName InSocketName, ERelativeTransformSpace TransformSpace = RTS_World) const override;
	virtual void QuerySupportedSockets(TArray<FComponentSocketDescription>& OutSockets) const override;

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// The name of the socket along the spline.
	static const FName SocketName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, BlueprintGetter = GetSplineSocketPositionKey, BlueprintSetter = SetSplineSocketPositionKey)
		float SplineSocketPositionKey;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float TransitionUpdateTickInterval = 0.01f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float TransitionDelay = 1.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float TransitionDuration = 2.0f;

	UFUNCTION(BlueprintGetter)
		float GetSplineSocketPositionKey();

	UFUNCTION(BlueprintSetter)
		void SetSplineSocketPositionKey(float SplineSocketPositionKeyValue);

	UFUNCTION(BlueprintCallable)
		void SetSplineSocketPositionKeyDelayed(float SplineSocketPositionKeyValue);

	virtual void PostInitProperties() override;

protected:

	void UpdateSplineSocket();

	bool bIsTransitioning = false;

	FVector RelativeSocketLocation = FVector::ZeroVector;

	FQuat RelativeSocketRotation = FQuat::Identity;

	FTimerHandle TransitionTimerHandle;

	UPROPERTY()
		ULerpUtility* LerpUtility;
};
