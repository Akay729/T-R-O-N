// Fill out your copyright notice in the Description page of Project Settings.


#include "TronDiskProjectile.h"

#include "GameFramework/ProjectileMovementComponent.h"


// Sets default values
ATronDiskProjectile::ATronDiskProjectile()
{
	ProjectileMovementComp->bShouldBounce = true;
	ProjectileMovementComp->Bounciness = 1.f;
}

void ATronDiskProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	ProjectileMovementComp->OnProjectileBounce.AddDynamic(this, &ThisClass::BounceBehavior);
}

void ATronDiskProjectile::BounceBehavior(const FHitResult& ImpactResult, const FVector& ImpactVelocity )
{
	//@todo:
	//- Applay sfx e vfx sul target.
	//- call detectTargetActor and determinate the direction of the disk
	
	BounceCounter++;
	if (BounceCounter >= MaxBounce)
	{
		StartRecallDisk();
	}
	
}

void ATronDiskProjectile::StartRecallDisk()
{
	// @todo:
	// - Move disk to current actor location
	// - it shoud go back go throug everythinh and applay damge to enemy
	// - could reset or reduce the a variabile that keep track of bounce
	// -
	
	AActor* DiskOwner = GetInstigator();
	if (!DiskOwner) 
	{
		UE_LOG(LogTemp, Warning, TEXT("StartRecallDisk: Instigator is null!"));
		return;
	}
	
	ProjectileMovementComp->bShouldBounce = false;
	ProjectileMovementComp->Velocity = FVector::ZeroVector;
	
	if (UPrimitiveComponent* CollisionComp = FindComponentByClass<UPrimitiveComponent>())
	{
		CollisionComp->SetCollisionResponseToAllChannels(ECR_Overlap);
	}
	
	GetWorldTimerManager().SetTimer(
		RecallTimerHandle,
		this,
		&ATronDiskProjectile::MoveTowardsOwner,
		0.02f,
		true
	);
	
}

void ATronDiskProjectile::MoveTowardsOwner()
{
	AActor* DiskOwner = GetInstigator();
	if (!DiskOwner)
	{
		GetWorldTimerManager().ClearTimer(RecallTimerHandle);
		return;
	}

	FVector TargetLocation = DiskOwner->GetActorLocation();
	FVector CurrentLocation = GetActorLocation();
	float Distance = FVector::Dist(CurrentLocation, TargetLocation);
	
	if (Distance < 80.f)
	{
		GetWorldTimerManager().ClearTimer(RecallTimerHandle);
		Destroy();
		return;
	}
	
	//Temporary solution
	//@todo: Implement a sweep for detect witch type of actor it went through...
	FVector Direction = (TargetLocation - CurrentLocation).GetSafeNormal();
	float Speed = 1500.f;
	FVector NewLocation = CurrentLocation + Direction * Speed * 0.02f;

	SetActorLocation(NewLocation, false, nullptr, ETeleportType::TeleportPhysics);
}

void ATronDiskProjectile::DetectTargetActor()
{
	//@todo
	//- Check if a target actor is in range.
	//- adjust direction rotation of the disk.
}



//@todo: Add a debug line and hitbox
