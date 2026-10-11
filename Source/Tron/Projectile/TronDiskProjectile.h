// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TronProjectile.h"
#include "TronDiskProjectile.generated.h"

UCLASS()
class TRON_API ATronDiskProjectile : public ATronProjectile
{
	GENERATED_BODY()

public:
	ATronDiskProjectile();

	void RecallDisk();
	void BounceBehavior(const FHitResult& ImpactResult, const FVector& ImpactVelocity );
	void DetectTargetActor();
	virtual void PostInitializeComponents() override;
	virtual void OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		FVector NormalImpulse, const FHitResult& Hit) override;
};


//@todo: 
//-Fucntion to recall the disk 
//-Function to handle bounce behavior - 
