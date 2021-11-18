// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "KaijuCamera.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class NEOKAIJU_API UKaijuCamera : public UCameraComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UKaijuCamera();

	UPROPERTY(BlueprintGetter = GetKaijuCameraAperture, BlueprintSetter = SetKaijuCameraAperture, EditAnywhere)
		float KaijuCameraAperture = 0.7f;

	UFUNCTION(BlueprintGetter)
		float GetKaijuCameraAperture() const;

	UFUNCTION(BlueprintSetter)
		void SetKaijuCameraAperture(float KaijuCameraApertureValue);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
