// Fill out your copyright notice in the Description page of Project Settings.


#include "SwarmGrabManager.h"

USwarmGrabManager::USwarmGrabManager() {
	availableNums = TArray<bool>();
	numAvailable = 512;
	for (int i = 0; i < numAvailable; i++) {
		availableNums.Add(false);
	}
	commandingSwarm = -1;
	numSwarmsGrabbing = 0;
	capturedPosition = 0;
	capturedVelocity = 0;
	capturedAcceleration = 0;
	capturedInitPosition = FVector(0,0,0);
}

int32 USwarmGrabManager::ObtainGrabNumber() {
	for (int i = 0; i < availableNums.Num(); i++) {
		if (availableNums[i] == false) {
			availableNums[i] = true;
			if(numSwarmsGrabbing <= 0){
				numSwarmsGrabbing = 1;
				commandingSwarm = i;
			}
			else {
				numSwarmsGrabbing++;
				if (commandingSwarm > i) {
					commandingSwarm = i;
				}
			}
			return (int32)i;
		}
	}
	return -1; //out of available numbers
}

void USwarmGrabManager::ReleaseGrabNumber(int32 grabNum) {
	if (availableNums[grabNum]) {
		availableNums[grabNum] = false;

		if (numSwarmsGrabbing > 0)
			numSwarmsGrabbing--;

		if (numSwarmsGrabbing <= 0)
			commandingSwarm = -1;
		else {
			for (int i = 0; i < availableNums.Num(); i++) {
				if (availableNums[i]) {
					commandingSwarm = i;
					break;
				}
			}
		}
	}
}

float USwarmGrabManager::SetCapturePosition(float newCapturePosition, int32 grabNum) {
	if (grabNum == commandingSwarm) {
		capturedPosition = newCapturePosition;
	}
	return capturedPosition;
}

float USwarmGrabManager::SetCaptureVelocity(float newCaptureVelocity, int32 grabNum) {
	if (grabNum == commandingSwarm) {
		capturedVelocity = newCaptureVelocity;
	}
	return capturedVelocity;
}

float USwarmGrabManager::SetCaptureAcceleration(float newCaptureAcceleration, int32 grabNum) {
	if (grabNum == commandingSwarm) {
		capturedAcceleration = newCaptureAcceleration;
	}
	return capturedAcceleration;
}

float USwarmGrabManager::SetCaptureTime(float newCaptureTime, int32 grabNum) {
	if (grabNum == commandingSwarm) {
		capturedTime = newCaptureTime;
	}
	return capturedTime;
}

FVector USwarmGrabManager::SetCaptureInitPosition(FVector newCaptureInitPosition, int32 grabNum) {
	if (grabNum == commandingSwarm) {
		capturedInitPosition = newCaptureInitPosition;
	}
	return capturedInitPosition;
}

void USwarmGrabManager::AddDashAcceleration(float increaseAccelerationAmount, int32 grabNum) {
	if(availableNums[grabNum])
		capturedAcceleration += increaseAccelerationAmount;
}