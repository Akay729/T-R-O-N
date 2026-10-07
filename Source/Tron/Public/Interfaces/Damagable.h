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
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Damage")
	float GetCurrentHealth();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Damage")
	float GetMaxHealth();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Damage")
	float Heal(float Amount);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Damage")
	bool ReciveDamage(FDamageInfo DamageInfo); //applay dmg and Return if was damage
	
};
