// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataTypes/DamageTypes.h"
#include "UObject/Interface.h"
#include "Damagable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UDamagable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TRON_API IDamagable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual float GetCurrentHealth() = 0;
	virtual float GetMaxHealth() = 0;
	virtual float Heal(float Amount) = 0;
	UFUNCTION()
	virtual bool ReciveDamage(FDamageInfo DamageInfo) = 0; //applay dmg and Return if was damage
	
};
