// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/AttributeComponent.h"

// Sets default values for this component's properties
UAttributeComponent::UAttributeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UAttributeComponent::BeginPlay()
{
	Super::BeginPlay();
	CurrentHealth = MaxHealth;
	CurrentArmor = FMath::Clamp(BaseArmor, 0.f, MaxArmor);
	CurrentStamina = MaxStamina;
}

// Called every frame
void UAttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UAttributeComponent::TakeDamage(float DamageAmount)
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

bool UAttributeComponent::IsDead()
{
	return CurrentHealth <= 0;
}

void UAttributeComponent::InitializeStats(float baseArmor, float maxArmor, float maxHealth, float maxStamina)
{
	BaseArmor =  FMath::Clamp(baseArmor, 0.f, 1.f);
	MaxArmor  = FMath::Clamp(maxArmor, 0.f, 1.f);
	MaxHealth = maxHealth;
	MaxStamina = maxStamina;
}

////////// ----------- ARMOR -----------//////////
float UAttributeComponent::ModifyCurrentArmor(float value)
{
	CurrentArmor = FMath::Clamp(CurrentArmor + value, BaseArmor, MaxArmor);
	return CurrentArmor;
}
float UAttributeComponent::ResetCurrentArmor()
{
	CurrentArmor = BaseArmor;
	return CurrentArmor;
}
float UAttributeComponent::GetCurrentArmor()
{
	return CurrentArmor;
}

////////// ----------- HEALTH -----------//////////
float UAttributeComponent::Heal(float HealAmount)
{
	if (IsDead()) return 0;
	CurrentHealth = FMath::Clamp(CurrentHealth + HealAmount, 0.f, MaxHealth);
	OnHealthChanged.Broadcast(CurrentHealth);
	return CurrentHealth;
}

float UAttributeComponent::GetCurrentHealth() const
{
	return CurrentHealth;
}

float UAttributeComponent::GetMaxHealth() const
{
	return MaxHealth;
}

float UAttributeComponent::GetHealthPercent() const
{
	return CurrentHealth / MaxHealth;
}

////////// ----------- STAMINA -----------//////////
void UAttributeComponent::RegenStamina(float StaminaValue)
{
	CurrentStamina = FMath::Clamp(CurrentStamina + StaminaValue, 0.0f, MaxStamina);
	if (CurrentStamina == MaxStamina) StopRegenStamina();
	UE_LOG(LogTemp, Warning, TEXT("StaminaRegenAmount: %f"), StaminaValue);
}

void UAttributeComponent::StartRegenStamina()
{
	if(CurrentStamina < MaxStamina)
	{
		StaminaDelegate.BindUObject(this, &UAttributeComponent::RegenStamina, StaminaRegen);
		GetWorld()->GetTimerManager().SetTimer(
			StaminaTimerHandle,
			StaminaDelegate,
			0.1,
			true
		);
	}
}

void UAttributeComponent::StopRegenStamina()
{
	GetWorld()->GetTimerManager().ClearTimer(StaminaTimerHandle);
}

bool UAttributeComponent::DrainStamina(float DrainValue)
{
	bool isdraining = !CurrentStamina == 0.0f;
	CurrentStamina = FMath::Clamp(CurrentStamina - DrainValue, 0.0f, MaxStamina);
	return isdraining;
}

float UAttributeComponent::GetMaxStamina() const
{
	return MaxStamina; 
}

float UAttributeComponent::GetCurrentStamina() const
{
	return CurrentStamina;
}

float UAttributeComponent::GetStaminaPercent() const
{
	return CurrentStamina / MaxStamina;
}
