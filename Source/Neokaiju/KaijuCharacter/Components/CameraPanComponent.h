// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "CameraPanComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class NEOKAIJU_API UCameraPanComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCameraPanComponent();


	// The name of the socket that is panned.
	static const FName SocketName;

	virtual bool HasAnySockets() const override;

	virtual FTransform GetSocketTransform(FName InSocketName, ERelativeTransformSpace TransformSpace = RTS_World) const override;

	virtual void QuerySupportedSockets(TArray<FComponentSocketDescription>& OutSockets) const override;

	UPROPERTY(BlueprintReadOnly)
		FVector CameraPanOffset = FVector::ZeroVector;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FVector MaxCameraOffset = FVector(100, 0, 100);

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		float PanSpeed = 1.0f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		float OriginReturnSpeed = 1.0f;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void BeginPlay() override;

protected:
	float GetReturnComponent(float AxisValue);
		
};
