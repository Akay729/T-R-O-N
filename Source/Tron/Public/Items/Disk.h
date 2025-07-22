// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DataTypes/DamageTypes.h"
#include "Disk.generated.h"

UENUM(BlueprintType)
enum class EDiskState : uint8 {
	None	UMETA(DisplayName = "None"),
	Attached	UMETA(DisplayName = "Attached"),
	Throw	UMETA(DisplayName = "Thrown"),
	Returning	UMETA(DisplayName = "Returning"),
	Deflected	UMETA(DisplayName = "Deflected")
};

class ABaseCharacter;

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

	void ApplayDamage(AActor* TargetActor, FDamageInfo DamageInfo);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	void DiskSweepTraceForTaget(FVector ViewpointLocation, FRotator ViewpointRotation);
	void Throw();
	void ReattachDiskToSocket();
	void DoMeleeAttack(FVector StartPosition, FVector EndPosition);

	UPROPERTY(EditDefaultsOnly)
	float DiskSphereRad = 20;
	
	UPROPERTY(EditDefaultsOnly)
	float MeleeAttackSphereRad = 20;
	
	UPROPERTY(EditDefaultsOnly)
	float MeleeDamage = 50;
	
	UPROPERTY(EditDefaultsOnly)
	float ThrowDamage = 100;
	
	UPROPERTY(EditDefaultsOnly)
	FDamageInfo MeleeDamageInfo;

	UPROPERTY(EditDefaultsOnly)
	FDamageInfo ThrowDamageInfo;
	
	UPROPERTY()
	EDiskState CurrentState = EDiskState::None;

	UFUNCTION(BlueprintCallable)
	EDiskState GetCurrentState() {return CurrentState;}

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Movement")
	class UProjectileMovementComponent* DiskMovementComponent;

private:

	AActor* DiskOwner;
	AController* OwnerController;
	ABaseCharacter* DiskCharacterOwner;

	UPROPERTY(VisibleAnywhere, Category="Costum")
	UStaticMeshComponent* DiskMeshComponent;

	UPROPERTY(EditDefaultsOnly)
	float DiskSpeed = 4000;
	
	UPROPERTY(EditDefaultsOnly)
	float DiskMaxSpeed = 5000;
	
	UPROPERTY(EditDefaultsOnly)
	float RangeToCatch = 150;
	
	UPROPERTY(EditDefaultsOnly)
	int32 MaxBounces = 3;
	
	UPROPERTY(VisibleAnywhere)
	int32 BounceCount = 0;
	
	FCollisionQueryParams Params;

	void GoBackToOwner();
	void UpdateVelocityToActorLocation(AActor* TargetActor);
	FTimerHandle DiskTrajectoryTimeHandle;
	FTimerDelegate DiskTimerDel;

	/* UFUNCTION()
	void DiskTrace(FHitResult &HitResult, FVector& ShotDirection); */
	
	UFUNCTION()
	AController* GetOwnerController();
};
