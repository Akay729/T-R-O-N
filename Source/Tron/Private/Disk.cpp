// Fill out your copyright notice in the Description page of Project Settings.


#include "Disk.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetMathLibrary.h"
#include "BaseCharacter.h"
// Sets default values
ADisk::ADisk()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DiskMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	RootComponent = DiskMeshComponent;

	DiskMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement"));
	DiskMovementComponent->MaxSpeed = DiskMaxSpeed;
	DiskMovementComponent->InitialSpeed = 0;
	DiskMovementComponent->Velocity = FVector::ZeroVector;
	DiskMovementComponent->ProjectileGravityScale = 0.0f;
	DiskMovementComponent->OnProjectileBounce.AddDynamic(this, &ADisk::OnProjectileBounce);

}

// Called when the game starts or when spawned
void ADisk::BeginPlay()
{
	Super::BeginPlay();
	DiskMovementComponent->StopMovementImmediately();
	CurrentState = EDiskState::Attached;
	
}

// Called every frame
void ADisk::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(CurrentState == EDiskState::Returning)
	{
		ReattachDiskToSocket();
		//CurrentState = EDiskState::Attached;
	}
	if (!DiskMovementComponent->IsActive())
	{
		UE_LOG(LogTemp, Warning, TEXT("Movement DISATTIVO"));
	}
	UE_LOG(LogTemp, Warning, TEXT("Disk Velocity: %s"), *DiskMovementComponent->Velocity.ToString());
}

int32 ADisk::GetDiskBounce()
{
	return BounceCount;
}

void ADisk::OnProjectileBounce(const FHitResult& ImpactResult, const FVector& ImpactVelocity)
{
    BounceCount++;
    
	//UE_LOG(LogTemp, Warning, TEXT("Rimbalzo #%d"), BounceCount);
	if (BounceCount >= MaxBounces)
	{	
		GoBackToOwner();
	}
}

void ADisk::Throw()
{
	AActor* DiskOwner = GetOwner();
	if (DiskOwner == nullptr) return;
	AController* OwnerController = GetOwnerController();
	if(OwnerController == nullptr) return;
	//if(CurrentState == EDiskState::Throw) return;
	
	if (CurrentState == EDiskState::Attached)
	{
		DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		
		DiskMovementComponent->StopMovementImmediately();
		DiskMovementComponent->Deactivate();
		DiskMovementComponent->SetUpdatedComponent(DiskMeshComponent); // ← importante
		DiskMovementComponent->SetComponentTickEnabled(true);
		DiskMeshComponent->IgnoreActorWhenMoving(GetOwner(), true);

		DiskMovementComponent->Activate(true);
		DiskMovementComponent->Velocity = FVector::ZeroVector; // reset prima di lanciare
		
		FVector	ViewpointLocation;
		FRotator ViewPortRotation;
		OwnerController->GetPlayerViewPoint(ViewpointLocation,ViewPortRotation);
		
		DiskMovementComponent->bShouldBounce = true;
		DiskMovementComponent->Bounciness = 1.f;
		DiskMovementComponent->Friction = 0.f;
		
		FVector LaunchDirection = ViewPortRotation.Vector();
		DiskMovementComponent->Velocity = LaunchDirection * DiskSpeed;
		SetActorRotation(LaunchDirection.Rotation());
		SetActorEnableCollision(true);
		
		CurrentState = EDiskState::Throw;
	}
	//UE_LOG(LogTemp, Display, TEXT("Owner: %s"), *DiskOwner->GetName());
}

void ADisk::GoBackToOwner()
{
	if (CurrentState == EDiskState::Throw)
	{
		AActor* DiskOwner = GetOwner();
		if (DiskOwner == nullptr) return;
		
		FVector OwnerLocation = DiskOwner->GetActorLocation();
		FVector DiskLocation = GetActorLocation();
		
		if(DiskMovementComponent == nullptr) return;
		DiskMovementComponent->bShouldBounce = false;
		//DiskMovementComponent->Velocity = FVector::ZeroVector;
		
		// Normalizza la direzione e scala per mantenere la stessa velocità
		FVector DirectionToOwner = (OwnerLocation - DiskLocation).GetSafeNormal();
		DiskMovementComponent->Velocity = DirectionToOwner * DiskSpeed;
		CurrentState = EDiskState::Returning;
	}
}

void ADisk::ReattachDiskToSocket()
{
	ABaseCharacter* DiskOwner = Cast<ABaseCharacter>(GetOwner());
	if (DiskOwner == nullptr) return;

	FVector SocketLocation = DiskOwner->GetMesh()->GetSocketLocation(TEXT("RightHandSocket"));
	if (FVector::Dist(GetActorLocation(), SocketLocation) < RangeToCatch)
	{
		bool isAttached = AttachToComponent(DiskOwner->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("RightHandSocket"));
		SetActorRotation(DiskOwner->GetMesh()->GetSocketRotation(TEXT("RightHandSocket")));

		SetActorEnableCollision(false);
		DiskMovementComponent->Velocity = FVector::ZeroVector;
		DiskMovementComponent->StopMovementImmediately();
		BounceCount = 0;
		
		if (isAttached)
		{
			CurrentState = EDiskState::Attached;
		}
		
	}
}

AController* ADisk::GetOwnerController()
{
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if(OwnerPawn) return OwnerPawn->GetController();
	return nullptr;
}