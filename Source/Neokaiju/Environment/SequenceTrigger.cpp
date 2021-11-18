// Fill out your copyright notice in the Description page of Project Settings.


#include "SequenceTrigger.h"
#include "LevelSequenceActor.h"
#include "LevelSequencePlayer.h"
#include "Neokaiju/Environment/SeamlessSequencePlayer.h"


void ASequenceTrigger::OnBeginTrigger(AActor* TriggeringActor)
{
	this->SequencePlayer = USeamlessSequencePlayer::CreateSeamlessSequencePlayerFromRefs(
		this,
		this->TriggeredSequence,
		this->TrigeredSequenceCamera,
		this->EaseInTime,
		this->EaseOutTime,
		this->bIsInputDisabledDuringSequence,
		this->EaseInBlendFunction,
		this->EaseOutBlendFunction,
		this->EaseInBlendExponent,
		this->EaseOutBlendExponent);

	this->SequencePlayer->PlaySequence();
	this->SequencePlayer->OnSeamlessSequencePlayerFinished.AddUniqueDynamic(this, &ASequenceTrigger::OnSequenceFinished);
}

void ASequenceTrigger::OnSequenceFinished()
{
	this->FinishTransition();
}

void ASequenceTrigger::OnEndTrigger(AActor* TriggeringActor)
{
	this->SequencePlayer->StopSequence();
	this->SequencePlayer = nullptr;
}
