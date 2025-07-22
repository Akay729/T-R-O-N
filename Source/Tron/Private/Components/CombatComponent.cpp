// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/CombatComponent.h"
#include "DataTypes/DamageTypes.h"
#include "Components/AttributeComponent.h"

// Sets default values for this component's properties
UCombatComponent::UCombatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();
	CompOwner = GetOwner();
	if(CompOwner)
	{
		AttributeComp = CompOwner->FindComponentByClass<UAttributeComponent>();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Miss Attribute Component"));
	}
	
}


// Called every frame
void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UCombatComponent::ReciveDamage(FDamageInfo DamageInfo, bool &bWasDamage)
{
	bWasDamage = false;
	if (!CompOwner || AttributeComp->IsDead()) return;

	//Invicible
	if (bIsInvincible && !DamageInfo.ShouldDamageInvincible) return;
	
	//Parry
	if (DamageInfo.CanBeParried && bIsParring)
	{
		UE_LOG(LogTemp, Warning, TEXT("Parried"));
		return;
	}

	//Block
	if (DamageInfo.CanBeBlocked && bIsBlocking)
	{
		AttributeComp->StopRegenStamina();
		//Event dipsacer
		OnBlocked.Broadcast(DamageInfo.CanBeParried);
		//TEST
		bool isDraining = AttributeComp->DrainStamina(20);
		if (isDraining)
		{
			UE_LOG(LogTemp, Warning, TEXT("Blocked"));
			GetWorld()->GetTimerManager().SetTimer(
				StartStaminaRegen,
				AttributeComp,
				&UAttributeComponent::StartRegenStamina,
				3.0f,
				false
			);
			return;
		}
	}

	//Interrupt
	if(bIsInterruptible && DamageInfo.ShouldForceInterrupt )
	{
		OnDamageResponse.Broadcast(DamageInfo.DmgResponse);
		//Interrompi Azione
	}

	AttributeComp->TakeDamage(DamageInfo.Amount);
	bWasDamage = true;
}

bool UCombatComponent::SetIsBlocking(bool value)
{
	bIsBlocking = value;
    return bIsBlocking;
}

bool UCombatComponent::SetIsInvincible(bool value)
{
	bIsInvincible = value;
    return bIsInvincible;
}

bool UCombatComponent::SetIsParring(bool value)
{
	bIsParring = value;
    return bIsParring;
}

bool UCombatComponent::SetIsInterruptible(bool value)
{
	bIsInterruptible = value;
    return bIsInterruptible;
}
