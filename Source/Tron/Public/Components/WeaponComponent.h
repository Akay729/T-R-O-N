// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponComponent.generated.h"


UENUM(BlueprintType)
enum class EAttacksState : uint8 {
	None	UMETA(DisplayName = "None"),
	StartAttack	UMETA(DisplayName = "StartAttack"),
	Attacking	UMETA(DisplayName = "Attacking"),
	AttackInterupt	UMETA(DisplayName = "AttackInterupt")
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TRON_API UWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWeaponComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	bool bEnabled;
	EAttacksState CurrentAttackState;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	virtual void Attack() PURE_VIRTUAL(UWeaponComponent::Attack, ); // PURE_VIRTUAL è un modo di unreal per dire = 0 xd
	virtual void StopAttack() PURE_VIRTUAL(UWeaponComponent::StopAttack, );
	virtual bool IsEnabled() const {return bEnabled;}
	virtual void SetEnabled(bool bNewState) { bEnabled = bNewState;}
	virtual EAttacksState GetCurrentAttackState() { return CurrentAttackState;}
		
};
