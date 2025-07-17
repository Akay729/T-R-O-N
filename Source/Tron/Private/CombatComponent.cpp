// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatComponent.h"
#include "DataTypes/DamageTypes.h"
#include "Components/HealthComponent.h"

// Sets default values for this component's properties
UCombatComponent::UCombatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();
	CompOwner = GetOwner();
	if(CompOwner)
	{
		HealthComp = CompOwner->FindComponentByClass<UHealthComponent>();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Miss Health Component"));
	}
	
}


// Called every frame
void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UCombatComponent::ReciveDamage(FDamageInfo DamageInfo)
{
	if (!CompOwner) return;
	
	if (HealthComp->IsDead()) return;

	if (bIsInvicible && !DamageInfo.ShouldDamageInvincible) return;
	
	if (DamageInfo.CanBeParried && bIsParring)
	{
		//Parry
	}

	if (DamageInfo.CanBeBlocked && bIsBlocking)
	{
		//Check stamina
		//if is enough block no damage taken and stamaina reduced
		//Else deplate stamina and take damage 

	}

	HealthComp->TakeDamage(DamageInfo.Amount);
}

bool UCombatComponent::SetIsBlocking(bool value)
{
	bIsBlocking = value;
    return bIsBlocking;
}

bool UCombatComponent::SetIsInvicible(bool value)
{
	bIsInvicible = value;
    return bIsInvicible;
}

bool UCombatComponent::SetIsParring(bool value)
{
	bIsParring = value;
    return bIsParring;
}
