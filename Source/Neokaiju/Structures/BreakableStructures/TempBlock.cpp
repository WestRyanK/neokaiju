// Fill out your copyright notice in the Description page of Project Settings.


#include "TempBlock.h"
#include "Neokaiju/Structures/BreakableStructures/BreakableStructureAttributeSet.h"

// Sets default values
ATempBlock::ATempBlock()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	this->AttributeSet = this->CreateDefaultSubobject<UBreakableStructureAttributeSet>(TEXT("AttributeSet"));
	this->AbilitySystemComponent = this->CreateDefaultSubobject<UAbilitySystemComponent>("AbilitySystemComponent");
}

UAbilitySystemComponent* ATempBlock::GetAbilitySystemComponent() const
{
	return this->AbilitySystemComponent;
}

void ATempBlock::Tick(float DeltaTime)
{
	//UE_LOG(LogTemp, Display, TEXT("Health %f"), this->AttributeSet->GetHealth());
}

// Called when the game starts or when spawned
void ATempBlock::BeginPlay()
{
	Super::BeginPlay();
	
	if (IsValid(this->InitialAttributeValues))
	{
		this->AbilitySystemComponent->ApplyGameplayEffectToSelf(this->InitialAttributeValues.GetDefaultObject(), 1, this->AbilitySystemComponent->MakeEffectContext());
	}

	this->ListenForAttributeChanges();
}

void ATempBlock::ListenForAttributeChanges()
{
	this->AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(this->AttributeSet->GetHealthAttribute()).AddUObject(this, &ATempBlock::HealthChanged);
}

void ATempBlock::HealthChanged(const FOnAttributeChangeData& Data)
{
	//UE_LOG(LogTemp, Display, TEXT("Health Changed from %f to %f"), Data.OldValue, Data.NewValue);
	if (Data.NewValue <= 0)
	{
		this->Destroy();
	}
}
