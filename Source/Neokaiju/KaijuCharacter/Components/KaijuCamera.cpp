// Fill out your copyright notice in the Description page of Project Settings.


#include "KaijuCamera.h"

UKaijuCamera::UKaijuCamera()
{
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UKaijuCamera::BeginPlay()
{
	Super::BeginPlay();
	this->SetKaijuCameraAperture(this->KaijuCameraAperture);
}


void UKaijuCamera::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	// Get the Y value of the Camera's location and set that as the focal distance, so the camera is always focused on the Y=0 plane
	FVector Location = this->GetComponentLocation();
	this->PostProcessSettings.DepthOfFieldFocalDistance = Location.Y;
	this->PostProcessSettings.bOverride_DepthOfFieldFocalDistance = true;

	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

float UKaijuCamera::GetKaijuCameraAperture() const
{
	return this->KaijuCameraAperture;
}

void UKaijuCamera::SetKaijuCameraAperture(float KaijuCameraApertureValue)
{
	this->KaijuCameraAperture = KaijuCameraApertureValue;
	this->PostProcessSettings.bOverride_DepthOfFieldFstop = true;
	this->PostProcessSettings.DepthOfFieldFstop = this->KaijuCameraAperture;
}
