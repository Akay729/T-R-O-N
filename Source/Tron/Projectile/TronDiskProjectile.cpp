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
	ProjectileMovementComp->OnProjectileBounce.AddDynamic(this, &ATronDiskProjectile::BounceBehavior);
}

void ATronDiskProjectile::RecallDisk()
{
	// @todo:
	// - Move disk to current actor location
	// - it shoud go back go throug everythinh and applay damge to enemy
	// - could reset or reduce the a variabile that keep track of bounce
	// -
	
	AActor* DiskOwner = GetInstigator();
	
	ProjectileMovementComp->HomingTargetComponent = DiskOwner->GetRootComponent();
	ProjectileMovementComp->bIsHomingProjectile = true;
	
}

void ATronDiskProjectile::BounceBehavior(const FHitResult& ImpactResult, const FVector& ImpactVelocity )
{
	//@todo:
	//- Keep track of bounce
	//- Applay sfx e vfx sul target.
	//- call detectTargetActor and determinate the direction of the disk
	//- Set collision to overlap all dynamics 
	//- Call recall with a timer in loop every 50/100 ms
	//- 
	
	BounceCounter++;
	if (BounceCounter >= MaxBounce)
	{
		RecallDisk();
	}
	
}

void ATronDiskProjectile::DetectTargetActor()
{
	//@todo
	//- Check if a target actor is in range.
	//- adjust direction rotation of the disk.
}



//@todo: Add a debug line and hitbox
