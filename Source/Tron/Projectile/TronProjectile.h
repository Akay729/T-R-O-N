// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TronProjectile.generated.h"

class UProjectileMovementComponent;
class USphereComponent;

UCLASS(Abstract)
class TRON_API ATronProjectile : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATronProjectile();
	
protected:
	
	
	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	TObjectPtr<USphereComponent> SphereComp;
	
	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComp;
	
	UPROPERTY(EditAnywhere, Category = "Projectile")
	float ProjectileCollisionRadius {15.0f};
	
	UFUNCTION()
	void PlayHitEffectsAtLocation();
public:
	virtual void PostInitializeComponents() override;
	virtual void OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		FVector NormalImpulse, const FHitResult& Hit );
};


//@todo:
//- vfx and sfx at location
//- confirmation sound hit for the player.