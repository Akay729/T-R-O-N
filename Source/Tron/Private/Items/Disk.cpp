// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Disk.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetMathLibrary.h"
#include "BaseCharacter.h"
#include "Components/HealthComponent.h"
#include "TimerManager.h"
// Sets default values
#define ECC_DiskTrace ECollisionChannel::ECC_GameTraceChannel1

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
	DrawDebugSphere(GetWorld(), GetActorLocation(), SphereRad, 36, FColor::Green, false, 0.1f);
	/* if(CurrentState == EDiskState::Returning)
	{
		ReattachDiskToSocket();
		//CurrentState = EDiskState::Attached;
	} */
}

int32 ADisk::GetDiskBounce()
{
	return BounceCount;
}

void ADisk::OnProjectileBounce(const FHitResult& ImpactResult, const FVector& ImpactVelocity)
{
    BounceCount++;
	
	AActor* HitActor = ImpactResult.GetActor();
	if (HitActor && this != HitActor && GetOwner() != HitActor)
	{
		if(HitActor->IsA(ABaseCharacter::StaticClass()))
		{
			GoBackToOwner();
			ApplayDamage(HitActor, 25.0f);
		}
		UE_LOG(LogTemp, Warning, TEXT("Target Valido"));
	}
	
	if (BounceCount >= MaxBounces)
	{	
		GoBackToOwner();
	}
}

void ADisk::ApplayDamage(AActor* TargetActor, float Amount)
{
	//Va reworkato con una iterface il prima possibile.
	UHealthComponent* HealtComp = TargetActor->FindComponentByClass<UHealthComponent>();
	if(HealtComp)
	{
		HealtComp->TakeDamage(Amount);
		UE_LOG(LogTemp, Warning, TEXT(" Actor: %s Danno subito: %f Da Actor:"), *TargetActor->GetName(), Amount, *GetOwner()->GetName());
	}
}

void ADisk::Throw()
{
	AActor* DiskOwner = GetOwner();
	if (DiskOwner == nullptr) return;
	AController* OwnerController = GetOwnerController();
	if(OwnerController == nullptr) return;
	
	// Get player viewpoint location and rotation 
	FVector	ViewpointLocation;
	FRotator ViewpointRotation;
	OwnerController->GetPlayerViewPoint(ViewpointLocation, ViewpointRotation);
	
	if (CurrentState == EDiskState::Attached)
	{
		DiskSweepTraceForTaget(ViewpointLocation, ViewpointRotation);
		DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		
		DiskMovementComponent->StopMovementImmediately();
		DiskMovementComponent->Deactivate();
		DiskMovementComponent->SetUpdatedComponent(DiskMeshComponent); // ← importante
		DiskMovementComponent->SetComponentTickEnabled(true);
		DiskMeshComponent->IgnoreActorWhenMoving(GetOwner(), true);

		DiskMovementComponent->Activate(true);
		DiskMovementComponent->Velocity = FVector::ZeroVector; // reset prima di lanciare
		
		DiskMovementComponent->bShouldBounce = true;
		DiskMovementComponent->Bounciness = 1.f;
		DiskMovementComponent->Friction = 0.f;
		
		FVector LaunchDirection = ViewpointRotation.Vector();
		DiskMovementComponent->Velocity = LaunchDirection * DiskSpeed;
		SetActorRotation(LaunchDirection.Rotation());
		SetActorEnableCollision(true);
		
		CurrentState = EDiskState::Throw;
	}
}

void ADisk::DiskSweepTraceForTaget(FVector ViewpointLocation, FRotator ViewpointRotation)
{
	FHitResult OutHit;
	FVector Start = ViewpointLocation;
	FVector End = Start + ViewpointRotation.Vector() * 1000;
	FCollisionShape SphereShape = FCollisionShape::MakeSphere(SphereRad);
	//DrawDebugCapsule()
	Params.AddIgnoredActor(GetOwner());
	Params.AddIgnoredActor(this);
	bool isHit = GetWorld()->SweepSingleByChannel(OutHit, Start, End, FQuat::Identity, ECC_DiskTrace, SphereShape, Params);
	if (isHit)
	{
		AActor* HitActor = OutHit.GetActor();
		if (HitActor)
		{
			UE_LOG(LogTemp, Warning, TEXT("Attore colpito: %s"), *HitActor->GetName());
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Colpito qualcosa, ma nessun attore valido"));
		}
	}
}

void ADisk::DiskSweep()
{
	//Crea uno sweep costante nella direzione del discoper garantire l'hit
}

void ADisk::GoBackToOwner()
{
	if (CurrentState == EDiskState::Throw)
	{
		AActor* DiskOwner = GetOwner();
		if (DiskOwner == nullptr) return;

		DiskTrajectoryTimeHandle;
		DiskTimerDel.BindUObject(this, &ADisk::UpdateVelocityToActorLocation, DiskOwner);
		GetWorldTimerManager().SetTimer(
			DiskTrajectoryTimeHandle,
			DiskTimerDel,
			0.075f,
			true
		);
		
		CurrentState = EDiskState::Returning;
	}
}

void ADisk::UpdateVelocityToActorLocation(AActor* TargetActor)
{
	//Questa funzione si puo usare sia per il lancio che per il ritorno.

	if (!TargetActor || !DiskMovementComponent) return;

	FVector TargetActorLocation = TargetActor->GetActorLocation();
	FVector DiskLocation = GetActorLocation();
	
	if(DiskMovementComponent == nullptr) return;
	DiskMovementComponent->bShouldBounce = false;
	//DiskMovementComponent->Velocity = FVector::ZeroVector;
	
	// Normalizza la direzione e scala per mantenere la stessa velocità
	FVector DirectionToTarget = (TargetActorLocation - DiskLocation).GetSafeNormal();
	DiskMovementComponent->Velocity = DirectionToTarget * DiskSpeed;
	ReattachDiskToSocket();
}

void ADisk::ReattachDiskToSocket()
{
	ABaseCharacter* DiskOwner = Cast<ABaseCharacter>(GetOwner());
	if (DiskOwner == nullptr) return;

	FVector SocketLocation = DiskOwner->GetMesh()->GetSocketLocation(TEXT("RightHandSocket"));
	if (FVector::Dist(GetActorLocation(), SocketLocation) < RangeToCatch)
	{
		GetWorldTimerManager().ClearTimer(DiskTrajectoryTimeHandle);
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

//Utils
AController* ADisk::GetOwnerController()
{
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if(OwnerPawn) return OwnerPawn->GetController();
	return nullptr;
}