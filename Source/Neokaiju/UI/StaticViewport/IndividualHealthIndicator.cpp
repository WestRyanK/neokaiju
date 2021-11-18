// Fill out your copyright notice in the Description page of Project Settings.


#include "IndividualHealthIndicator.h"

void UIndividualHealthIndicator::SetPercentage(float newValue)
{
	currentPercentage = newValue;
	RefreshUI();
}