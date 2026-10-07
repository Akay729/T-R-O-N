// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DamageTypes.generated.h"

/**
 * 
 */
UCLASS()
class TRON_API UDamageTypes : public UObject
{
	GENERATED_BODY()
};

UENUM(BlueprintType)
enum class DmgTypes : uint8
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
enum class DmgResponses : uint8
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
	DmgTypes DmgType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	DmgResponses DmgResponse;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool ShouldDamageInvincible; //Damage even if mid dodge

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool CanBeBlocked;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool CanBeParried;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool ShouldForceInterrupt;

};
