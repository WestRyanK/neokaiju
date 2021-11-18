// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseTagWatcher.h"

void UBaseTagWatcher::Init(UWorld* WorldValue)
{
	this->World = WorldValue;
}

void UBaseTagWatcher::TagSeen(FName SeenTag)
{
	this->OnTagSeen.Broadcast(SeenTag);
}
