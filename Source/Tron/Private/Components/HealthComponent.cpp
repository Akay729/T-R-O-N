// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/HealthComponent.h"
//#include "HealthComponent.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	CurrentHealth = MaxHealth;
}


// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UHealthComponent::TakeDamage(float DamageAmount)
{
	if (IsDead()) return;

	float DamageAfterArmor = FMath::Max(DamageAmount - DamageAmount * Armor, 0.f);
	CurrentHealth = FMath::Clamp(CurrentHealth - DamageAfterArmor, 0.f, MaxHealth);

	OnHealthChanged.Broadcast(CurrentHealth);

	if (IsDead())
	{
		OnDeath.Broadcast();
	}
}

void UHealthComponent::Heal(float HealAmount)
{
	if (IsDead()) return;
	CurrentHealth = FMath::Clamp(CurrentHealth + HealAmount, 0.f, MaxHealth);
}

bool UHealthComponent::IsDead()
{
    return CurrentHealth <= 0;
}

float UHealthComponent::GetHealthPercent() const
{
    return CurrentHealth / MaxHealth;
}
