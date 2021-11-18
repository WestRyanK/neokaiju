// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
//#include "MessagingSubsystem.generated.h"
//
//class UMessage;
//
//DECLARE_DELEGATE_OneParam(FSubscriberReceiveMessage, UMessage*, Message);
///**
//* Using a Messaging Subsystem seemed like a better alternative to a bunch of subsystems with their own delegates, but
//* it has turned out to increase the amount of code that people have to write without giving many benefits. So I'm marking
//* this class as deprecated so that no one uses it. I don't want to delete it though because there was substantial amount
//* of time invested and I learned a lot while doing it so I want to retain that knowledge here.
//*/
//UCLASS()
//class NEOKAIJU_API UMessagingSubsystem : public UGameInstanceSubsystem
//{
//	GENERATED_BODY()
//
//public:
//	virtual void Initialize(FSubsystemCollectionBase& Collection);
//
//	UFUNCTION(BlueprintCallable)
//		FSubscriberReceiveMessage Subscribe(FString MessageName);
//		//void Subscribe(FString MessageName, FSubscriberReceiveMessage Subscriber);
//
//UFUNCTION(BlueprintCallable)
//	void Unsubscribe(FString MessageName, FSubscriberReceiveMessage Subscriber);
//
//	UFUNCTION(BlueprintCallable)
//		void PublishMessage(UMessage* Message);
//
//private:
//	TMap<FString, TSet<FSubscriberReceiveMessage>> Subscribers;
//};
//
// // Use example:
//UMessagingSubsystem* Subsystem = this->GetWorld()->GetGameInstance()->GetSubsystem<UMessagingSubsystem>();
//Subsystem->Subscribe(UStructureDestroyedMessage::MESSAGE_NAME).BindUObject(this, &URegenerateHealthAbility::OnStructureDestroyed);
