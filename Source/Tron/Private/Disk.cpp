// Fill out your copyright notice in the Description page of Project Settings.


#include "Disk.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
ADisk::ADisk()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DiskMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	RootComponent = DiskMeshComponent;

	DiskMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement"));
	DiskMovementComponent->MaxSpeed = 10000.0f;
	DiskMovementComponent->InitialSpeed = 10000.0f;
}

// Called when the game starts or when spawned
void ADisk::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADisk::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

