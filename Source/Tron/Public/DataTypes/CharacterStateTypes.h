// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CharacterStateTypes.generated.h"

/**
 * 
 */
UCLASS()
class TRON_API UCharacterStateTypes : public UObject
{
	GENERATED_BODY()
	
};

UENUM(BlueprintType)
enum class EMovementState : uint8 
{
	None UMETA(DisplayName = "None"),
	Idle UMETA(DisplayName = "Walking"),
	Walking UMETA(DisplayName = "Walking"),
	Jogging UMETA(DisplayName = "Jogging"),
	Running UMETA(DisplayName = "Running"),
	Jumping UMETA(DisplayName = "Jumping")
};

UENUM(BlueprintType)
enum class EAttackState : uint8 
{
	None UMETA(DisplayName = "None"),
	Attacking UMETA(DisplayName = "Attacking"),
	ThrowingDisk UMETA(DisplayName = "ThrowingDisk"),
	Dashing UMETA(DisplayName = "Dashing"),
	Parrying UMETA(DisplayName = "Parrying"),
	Blocking UMETA(DisplayName = "Blocking")
};