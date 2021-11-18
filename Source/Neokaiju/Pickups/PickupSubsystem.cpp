// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupSubsystem.h"

void UPickupSubsystem::PickupPickedUp(APickupBase* PickupPickedUp)
{
	this->OnPickupPickedUp.Broadcast(PickupPickedUp);
}
