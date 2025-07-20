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
	CurrentArmor = FMath::Clamp(BaseArmor, 0.f, MaxArmor);
}

// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UHealthComponent::TakeDamage(float DamageAmount)
{
	if (IsDead()) return;

	float ArmorAbsorption = FMath::Clamp(CurrentArmor, 0.f, 1.f);
	float DamageAfterArmor = DamageAmount * (1.f - ArmorAbsorption);
	CurrentHealth = FMath::Clamp(CurrentHealth - DamageAfterArmor, 0.f, MaxHealth);

	OnHealthChanged.Broadcast(CurrentHealth);

	if (IsDead())
	{
		AActor* CompOwner = GetOwner();
		if(CompOwner) CompOwner->K2_DestroyActor();
		OnDeath.Broadcast();
	}
}

bool UHealthComponent::IsDead()
{
	return CurrentHealth <= 0;
}

void UHealthComponent::InitializeStats(float baseArmor, float maxArmor, float maxHealth, float maxStamina)
{
	BaseArmor =  FMath::Clamp(baseArmor, 0.f, 1.f);
	MaxArmor  = FMath::Clamp(maxArmor, 0.f, 1.f);
	MaxHealth = maxHealth;
	MaxStamina = maxStamina;
}

// Armor
float UHealthComponent::ModifyCurrentArmor(float value)
{
	CurrentArmor = FMath::Clamp(CurrentArmor + value, BaseArmor, MaxArmor);
	return CurrentArmor;
}
float UHealthComponent::ResetCurrentArmor()
{
	CurrentArmor = BaseArmor;
	return CurrentArmor;
}
float UHealthComponent::GetCurrentArmor()
{
	return CurrentArmor;
}

// Heal
float UHealthComponent::Heal(float HealAmount)
{
	if (IsDead()) return 0;
	CurrentHealth = FMath::Clamp(CurrentHealth + HealAmount, 0.f, MaxHealth);
	OnHealthChanged.Broadcast(CurrentHealth);
	return CurrentHealth;
}

float UHealthComponent::GetCurrentHealth() const
{
	return CurrentHealth;
}

float UHealthComponent::GetMaxHealth() const
{
	return MaxHealth;
}

float UHealthComponent::GetHealthPercent() const
{
	return CurrentHealth / MaxHealth;
}
