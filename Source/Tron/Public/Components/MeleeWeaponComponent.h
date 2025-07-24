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
	void SweepAttack();


private:
	UPROPERTY()
	FTimerHandle MeleeTimerHandle;
};