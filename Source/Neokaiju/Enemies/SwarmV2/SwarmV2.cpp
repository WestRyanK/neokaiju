// Fill out your copyright notice in the Description page of Project Settings.


#include "SwarmV2.h"
#include "AbilitySystemComponent.h"
#include "Neokaiju/Enemies/SwarmV2/SwarmState.h"
#include "Neokaiju/Enemies/SwarmV2/SwarmStats.h"
#include "Neokaiju/KaijuCharacter/KaijuCharacter.h"
#include "Components/SphereComponent.h"
#include "Neokaiju/Enemies/EnemySubsystem.h"


// Sets default values
ASwarmV2::ASwarmV2()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	NumDirs = 24;


	LastDirection = FVector(0, 0, 0);

	InnerRadius = CreateDefaultSubobject<USphereComponent>(TEXT("InnerRadius"));
	RootComponent = InnerRadius;

	OuterRadius = CreateDefaultSubobject<USphereComponent>(TEXT("OuterRadius"));
	OuterRadius->SetupAttachment(RootComponent);

	BulletMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BulletMesh"));
	BulletMesh->SetupAttachment(RootComponent);

	ClawMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ClawMesh"));
	ClawMesh->SetupAttachment(RootComponent);

	this->AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
}

// Called when the game starts or when spawned
void ASwarmV2::BeginPlay()
{
	Super::BeginPlay();
	
	MakeDirections();

	Init(); // this is for doing things I don't know how to do in C++
	// I am lasy, and a coward. And for that, Ryan, I apologize.
	

	this->ListenForTags();
}

void ASwarmV2::MakeDirections()
{

	Directions.Add(FRotator(0, 0, 0));
	float angleInc = 360.0f / NumDirs;
	float angle = angleInc;
	float oppAngle = 0.0f;
	for (int i = 0; i < NumDirs/2 -1; i++) {
		oppAngle = 360.0f - angle;
		Directions.Add(FRotator(angle, 0, 0));//pitch,yaw,roll instead of normal ue4
		Directions.Add(FRotator(oppAngle, 0, 0));
		angle += angleInc;
	}
	Directions.Add(FRotator(0, 180, 0));
}

// Called every frame
void ASwarmV2::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(IsValid(CurrentState) && IsValid(this->CurrentState->SwarmRef))
		CurrentState->Update(DeltaTime);
}

// Called to bind functionality to input
void ASwarmV2::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	//Super::SetupPlayerInputComponent(PlayerInputComponent);

	/* let me rant for a sec. you are a pawn. Why are you telling me AActor has no such function
	thank you for coming to my ted talk.*/
}

void ASwarmV2::ChangeState(USwarmState* NewState)
{
	if(CurrentState)
		CurrentState->Exit();

	CurrentState = NewState;

	if(CurrentState)
		CurrentState->Enter(this);
}

void ASwarmV2::Move(float DeltaTime, float Speed, FVector Target)
{
	FVector newLocation = Target.GetSafeNormal() * Speed * DeltaTime + GetActorLocation();
	SetActorLocation(newLocation);
}

FVector ASwarmV2::FindOpenSpace_Implementation(FVector Target,float MaxDist)
{
	return FVector(0,0,0);
}

FVector ASwarmV2::FindOpenSpaceFrontBack_Implementation(FVector Target, float MaxDist)
{
	return FVector(0, 0, 0);
}

void ASwarmV2::OrientSwarm_Implementation(FRotator rot) {
	SetActorRotation(rot);
}

void ASwarmV2::RotateSwarm_Implementation(float deltaTime) {
	return;
}

void ASwarmV2::AssembleSwarm_Implementation(float transformTime, float totalTransformTime) {
	SetActorRelativeScale3D(FVector(2.0f, 2.0f, 2.0f));
}

void ASwarmV2::DissembleSwarm_Implementation(float transformTime, float totalTransformTime) {
	SetActorRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));
}

void ASwarmV2::ScatterSwarmOut_Implementation(float scatterTime, float totalScatterTime)
{
	return;
}

void ASwarmV2::ScatterSwarmIn_Implementation(float scatterTime, float totalScatterTime)
{
	return;
}

FVector ASwarmV2::FindTargetNearKaiju(float TargetDistance)
{
	return FVector();
}

FVector ASwarmV2::FindStrafingTarget(bool ClockWise)
{
	return FVector();
}

UAbilitySystemComponent* ASwarmV2::GetAbilitySystemComponent() const
{
	return this->AbilitySystemComponent;
}

void ASwarmV2::ListenForTags()
{
	this->AbilitySystemComponent->RegisterGameplayTagEvent(this->HitByLaserTag, EGameplayTagEventType::NewOrRemoved)
		.AddUObject(this, &ASwarmV2::HitByLaserTagChanged);
	this->AbilitySystemComponent->RegisterGameplayTagEvent(this->OverlapByLaserTag, EGameplayTagEventType::NewOrRemoved)
		.AddUObject(this, &ASwarmV2::OverlapByLaserTagChanged);
}

void ASwarmV2::HitByLaserTagChanged(FGameplayTag Tag, int32 Count)
{
	if (Count == 1)
	{
		this->BeginHitByLaser();
	}
	else if (Count == 0)
	{
		this->EndHitByLaser();
	}
}


void ASwarmV2::OverlapByLaserTagChanged(FGameplayTag Tag, int32 Count)
{
	if (Count == 1)
	{
		this->BeginOverlapByLaser();
	}
	else if (Count == 0)
	{
		this->EndOverlapByLaser();
	}
}

UEnemySubsystem* ASwarmV2::GetEnemySubsystem() {
	UEnemySubsystem* EnemySubsystem = this->GetWorld()->GetSubsystem<UEnemySubsystem>();
	return EnemySubsystem;
}
