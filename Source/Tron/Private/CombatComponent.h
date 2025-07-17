// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DataTypes/DamageTypes.h"
#include "CombatComponent.generated.h"

class UHealthComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCombatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UFUNCTION()
	void ReciveDamage(FDamageInfo DamageInfo);

	UFUNCTION()
	bool SetIsBlocking(bool value);
	UFUNCTION()
	bool SetIsInvicible(bool value);
	UFUNCTION()
	bool SetIsParring(bool value);
	
private:
	UHealthComponent* HealthComp;
	AActor* CompOwner;

	bool bIsBlocking;
	bool bIsInvicible;
	bool bIsParring;
};
