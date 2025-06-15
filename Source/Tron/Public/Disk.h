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

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere, Category="Costum")
	UStaticMeshComponent* DiskMeshComponent;

	UPROPERTY(VisibleAnywhere, Category="Movement")
	class UProjectileMovementComponent* DiskMovementComponent;
};
