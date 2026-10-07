// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WeaponComponent.h"
#include "MeleeWeaponComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TRON_API UMeleeWeaponComponent : public UWeaponComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMeleeWeaponComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void Attack() override;
	virtual void StopAttack() override;
	virtual EAttacksState GetCurrentAttackState() override;
	void StartAttack();
	bool SweepAttack(FVector Origin, FVector Forward, float Radius, TSet<AActor*>& AlreadyHit);
	AController* GetOwnerController();


private:
	UPROPERTY()
	FTimerHandle MeleeTimerHandle;

	AActor* Owner;
	AActor* TopLevelOwner;
	AController* OwnerController;

	float SphereRadius = 50.0f;
	float SweepDistance = 125.0f;
	//int NumTraces = 8;
	//float ArcAngle = 90.0f;
	FCollisionQueryParams Params;
};