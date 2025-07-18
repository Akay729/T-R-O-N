// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DataTypes/DamageTypes.h"
#include "CombatComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBlocked, bool, WasParryable);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDamageResponse, DmgResponses, Response);

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
	void ReciveDamage(FDamageInfo DamageInfo, bool &bWasDamage);

	UFUNCTION(BlueprintCallable)
	bool SetIsBlocking(bool value);
	UFUNCTION(BlueprintCallable)
	bool SetIsInvincible(bool value);
	UFUNCTION(BlueprintCallable)
	bool SetIsParring(bool value);
	UFUNCTION(BlueprintCallable)
	bool SetIsInterruptible (bool value);

	//Delgates
	UPROPERTY()
	FOnBlocked OnBlocked;
	UPROPERTY()
	FOnDamageResponse OnDamageResponse;
	
private:
	
	UPROPERTY()
	UHealthComponent* HealthComp;
	
	UPROPERTY()
	AActor* CompOwner;
	
	UPROPERTY()
	bool bIsBlocking;
	UPROPERTY()
	bool bIsInvincible;
	UPROPERTY()
	bool bIsParring;
	UPROPERTY()
	bool bIsInterruptible;
};
