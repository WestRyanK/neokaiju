// Fill out your copyright notice in the Description page of Project Settings.


#include "RubbleManager.h"
#include "GameFramework/Actor.h"

void URubbleManager::AddRubble(AActor* Rubble)
{
	if (TotalMeshes > 0)
	{
		while (!ActiveMeshes.Peek())
		{
			RemoveRubble();
		}
	}

	ActiveMeshes.Enqueue(Rubble);
	TotalMeshes++;
	
	if (TotalMeshes > MaxMeshes)
	{
		RemoveRubble();
	}
}

void URubbleManager::RemoveRubble()
{
	AActor* OldRubble;
	ActiveMeshes.Dequeue(OldRubble);
	TotalMeshes--;

	if (OldRubble)
	{
		OldRubble->Destroy();
	}
}
