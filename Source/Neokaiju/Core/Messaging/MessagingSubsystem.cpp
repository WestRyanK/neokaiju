// Fill out your copyright notice in the Description page of Project Settings.


#include "MessagingSubsystem.h"
//#include "Neokaiju/Core/Messaging/Message.h"
//
//void UMessagingSubsystem::Initialize(FSubsystemCollectionBase& Collection)
//{
//	Super::Initialize(Collection);
//}
//void UMessagingSubsystem::PublishMessage(UMessage* Message)
//{
//	TSet<FSubscriberReceiveMessage>* SubscribersToMessage = this->Subscribers.Find(Message->GetMessageName());
//	if (SubscribersToMessage)
//	{
//		for (FSubscriberReceiveMessage Subscriber : *SubscribersToMessage)
//		{
//			Subscriber.ExecuteIfBound(Message);
//		}
//	}
//}
//
//
//FSubscriberReceiveMessage UMessagingSubsystem::Subscribe(FString MessageName)
//{
//	TSet<FSubscriberReceiveMessage>* SubscribersToMessage = this->Subscribers.Find(MessageName);
//
//	FSubscriberReceiveMessage Subscriber;
//	if (!SubscribersToMessage)
//	{
//		TSet<FSubscriberReceiveMessage> SubscriberSet = TSet<FSubscriberReceiveMessage>();
//		SubscriberSet.Add(Subscriber);
//		this->Subscribers.Add(MessageName, SubscriberSet);
//	}
//	else
//	{
//		SubscribersToMessage->Add(Subscriber);
//	}
//	return Subscriber;
//}
//
//void UMessagingSubsystem::Unsubscribe(FString MessageName, FSubscriberReceiveMessage Subscriber)
//{
//	TSet<FSubscriberReceiveMessage>* SubscribersToMessage = this->Subscribers.Find(MessageName);
//	if (SubscribersToMessage)
//	{
//		SubscribersToMessage->Remove(Subscriber);
//	}
//}
//
