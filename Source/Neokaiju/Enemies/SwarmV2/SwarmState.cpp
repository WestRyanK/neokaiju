// Fill out your copyright notice in the Description page of Project Settings.


#include "SwarmState.h"
#include "Neokaiju/Enemies/SwarmV2/SwarmV2.h"

USwarmState::USwarmState()
{

}

void USwarmState::Enter_Implementation(ASwarmV2* NewSwarm)
{
	this->SwarmRef = NewSwarm;
}