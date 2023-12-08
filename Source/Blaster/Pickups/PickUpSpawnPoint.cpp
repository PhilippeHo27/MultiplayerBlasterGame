// Fill out your copyright notice in the Description page of Project Settings.


#include "PickUpSpawnPoint.h"
#include "Pickup.h"

APickUpSpawnPoint::APickUpSpawnPoint()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
}

void APickUpSpawnPoint::BeginPlay()
{
	Super::BeginPlay();
	//StartSpawnPickupTimer((AActor*)nullptr);
	StartSpawnPickupTimer(nullptr);
}

void APickUpSpawnPoint::SpawnPickup()
{
	int32 NumPickupClasses = PickupClasses.Num();
	if (NumPickupClasses > 0)
	{
		int32 Selection = FMath::RandRange(0, NumPickupClasses - 1);
		SpawnedPickup = GetWorld()->SpawnActor<APickup>(PickupClasses[Selection], GetActorTransform());

		if (HasAuthority() && SpawnedPickup)
		{
			SpawnedPickup->OnDestroyed.AddDynamic(this, &APickUpSpawnPoint::StartSpawnPickupTimer);
		}
	}
}

void APickUpSpawnPoint::SpawnPickupTimerFinished()
{
	if (HasAuthority())
	{
		SpawnPickup();
	}
}

void APickUpSpawnPoint::StartSpawnPickupTimer(AActor* DestroyedActor)
{
	const float SpawnTime = FMath::FRandRange(SpawnPickupTimeMin, SpawnPickupTimeMax);
	GetWorldTimerManager().SetTimer(
		SpawnPickupTimer,
		this,
		&APickUpSpawnPoint::SpawnPickupTimerFinished,
		SpawnTime
	);
}

void APickUpSpawnPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
