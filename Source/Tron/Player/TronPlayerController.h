// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Controller.h"
#include "TronPlayerController.generated.h"

class UInputAction;
class ATronDiskProjectile;

UCLASS()
class TRON_API ATronPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UInputAction> ThrowDiskAction;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ATronDiskProjectile> DiskProjectile;
	
	virtual void SetupInputComponent() override ;

public:
	ATronPlayerController();
	void ThrowDisk();
};
