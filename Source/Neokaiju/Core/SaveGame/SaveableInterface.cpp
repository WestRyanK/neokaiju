// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveableInterface.h"
#include "Engine/Level.h"

FString ISaveableInterface::GetSaveKey()
{
	FString ActorName = this->_getUObject()->GetName();
	return ActorName;
}

FString ISaveableInterface::GetLevelKey()
{
	FString LevelName = this->_getUObject()->GetTypedOuter(ULevel::StaticClass())->GetOuter()->GetName();
	return LevelName;
}
