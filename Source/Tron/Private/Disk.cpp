// Fill out your copyright notice in the Description page of Project Settings.


#include "Disk.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ADisk::ADisk()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DiskMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	RootComponent = DiskMeshComponent;

	DiskMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement"));
	DiskMovementComponent->MaxSpeed = DiskMaxSpeed;
	DiskMovementComponent->InitialSpeed = DiskInitialSpeed;
	DiskMovementComponent->bShouldBounce = true;
	DiskMovementComponent->Bounciness = 1.f;
	DiskMovementComponent->ProjectileGravityScale = 0.0f;
	DiskMovementComponent->OnProjectileBounce.AddDynamic(this, &ADisk::OnProjectileBounce);
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

	FVector Start = GetActorLocation();
	FVector WorldDir = GetVelocity();
	//FVector Direction = UKismetMathLibrary::InverseTransformDirection(GetActorTransform(), WorldDir);
	FVector Direction = GetActorRotation().Vector();
	FVector End = Start + Direction * 500.0f;
	
	DrawDebugDirectionalArrow(GetWorld(), Start, End, 25.f, FColor::Green, false, 0.1f);
}

int32 ADisk::GetDiskBounce()
{
	return BounceCount;
}

void ADisk::OnProjectileBounce(const FHitResult& ImpactResult, const FVector& ImpactVelocity)
{
    BounceCount++;
    UE_LOG(LogTemp, Warning, TEXT("Rimbalzo #%d"), BounceCount);
	if (BounceCount >= MaxBounces)
	{	
		GoBackToOwner();
	}
}

void ADisk::GoBackToOwner()
{
	AActor* DiskOwner = GetOwner();
	if (DiskOwner == nullptr) return;
	
	FVector OwnerLocation =  DiskOwner->GetActorLocation();
	FVector DiskLocation = GetActorLocation();

	DiskMovementComponent->bShouldBounce = false;
	DiskMovementComponent->Velocity = FVector::ZeroVector;
	// Normalizza la direzione e scala per mantenere la stessa velocità
	FVector DirectionToOwner = (OwnerLocation - DiskLocation).GetSafeNormal();
	DiskMovementComponent->Velocity = DirectionToOwner * DiskMovementComponent->InitialSpeed;

}