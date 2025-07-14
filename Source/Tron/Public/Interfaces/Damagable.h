// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Damagable.generated.h"

UENUM(BlueprintType)
enum class DamageTypes : uint8
{
	// 
	None UMETA(DisplayName="None"),
	DiskMelee UMETA(DisplayName="DiskMelee"),
	DiskThrow UMETA(DisplayName="DiskThrow"),
	Punch UMETA(DisplayName="Punch"),
	Ability UMETA(DisplayName="Ability"),
	AbilityAOE UMETA(DisplayName="AbilityAOE"),
	Envirement UMETA(DisplayName="Envirement")
};

UENUM(BlueprintType)
enum class DamageResponses : uint8
{
	None UMETA(DisplayName="None"),
	HitReaction UMETA(DisplayName="HitReaction"),
	Stagger UMETA(DisplayName="Stagger"),
	KnockBack UMETA(DisplayName="KnockBack"),
	Slowed UMETA(DisplayName="Slowed"),
	Poisoned UMETA(DisplayName="Poisoned")
};

USTRUCT(BlueprintType)
struct FDamageInfo
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float Amount;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	DamageTypes DamageType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	DamageResponses DamageResponse;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool ShouldDamageInvincible; //Damage even if mid dodge

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool CanBeBlocked;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool CanBeParried;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool ShouldForceInterrupt;

};

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
	virtual bool TakeDamage(FDamageInfo DamageInfo) = 0; //applay dmg and Return if was damage
	
};
