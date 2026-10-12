// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TronMeleeWeapon.generated.h"

UCLASS()
class TRON_API ATronMeleeWeapon : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATronMeleeWeapon();
};

//@todo: 
//- Create socket basic collison rule and box for the melee weapon
//- Create a function for OnHitActior
//- Create fucktion for sfx and vfx when get hit.
//- add animation montage varriabile
//

