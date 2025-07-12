// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Disk.generated.h"

UENUM(BlueprintType)
enum class EDiskState : uint8 {
	None	UMETA(DisplayName = "None"),
	Attached	UMETA(DisplayName = "Attached"),
	Throw	UMETA(DisplayName = "Thrown"),
	Returning	UMETA(DisplayName = "Returning"),
	Deflected	UMETA(DisplayName = "Deflected")
};


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

	void ApplayDamage(AActor* TargetActor, float Amount);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	void Throw();
	void ReattachDiskToSocket();

	UPROPERTY()
	EDiskState CurrentState = EDiskState::None;

	UFUNCTION(BlueprintCallable)
	EDiskState GetCurrentState() {return CurrentState;}

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Movement")
	class UProjectileMovementComponent* DiskMovementComponent;

private:
	UPROPERTY(VisibleAnywhere, Category="Costum")
	UStaticMeshComponent* DiskMeshComponent;

	UPROPERTY(EditDefaultsOnly)
	float DiskSpeed = 1400;
	
	UPROPERTY(EditDefaultsOnly)
	float DiskMaxSpeed = 2000;
	
	UPROPERTY(EditDefaultsOnly)
	float RangeToCatch = 150;
	
	UPROPERTY(EditDefaultsOnly)
	int32 MaxBounces = 3;
	
	UPROPERTY(VisibleAnywhere)
	int32 BounceCount = 0;
	
	void GoBackToOwner();

	/* UFUNCTION()
	void DiskTrace(FHitResult &HitResult, FVector& ShotDirection); */
	
	UFUNCTION()
	AController* GetOwnerController();
};
