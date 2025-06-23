// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Disk.generated.h"

UCLASS()
class TRON_API ADisk : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADisk();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	int32 GetDiskBounce();

	UFUNCTION()
	void OnProjectileBounce(const FHitResult& ImpactResult, const FVector& ImpactVelocity);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere, Category="Costum")
	UStaticMeshComponent* DiskMeshComponent;

	UPROPERTY(VisibleAnywhere, Category="Movement")
	class UProjectileMovementComponent* DiskMovementComponent;

	UPROPERTY(EditDefaultsOnly)
	float DiskInitialSpeed = 600;
	UPROPERTY(EditDefaultsOnly)
	float DiskMaxSpeed = 1000;
	UPROPERTY(EditDefaultsOnly)
	int32 MaxBounces = 5;
	UPROPERTY(VisibleAnywhere)
	int32 BounceCount = 0;

	void GoBackToOwner();
};
