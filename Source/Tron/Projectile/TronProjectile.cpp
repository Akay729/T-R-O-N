// Fill out your copyright notice in the Description page of Project Settings.


#include "TronProjectile.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"


// Sets default values
ATronProjectile::ATronProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SphereComp->SetupAttachment(RootComponent);
	SphereComp->SetSphereRadius(ProjectileCollisionRadius);
	
	ProjectileMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComp"));
	ProjectileMovementComp->InitialSpeed = 1000.f;
	ProjectileMovementComp->ProjectileGravityScale = 0.0f;
	InitialLifeSpan = 10.f;
}

void ATronProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	SphereComp->OnComponentHit.AddDynamic(this, &ThisClass::OnActorHit);
	SphereComp->IgnoreActorWhenMoving(GetInstigator(), true);
	
}

void ATronProjectile::OnActorHit(UPrimitiveComponent*, HitComponent, AActor*, OtherActor, UPrimitiveComponent*,
	OtherComp, FVector, NormalImpulse, const FHitResult&, Hit) const
{
	
}


void ATronProjectile::PlayHitEffectsAtLocation()
{
	//@todo HitEffects AtLocation
}

