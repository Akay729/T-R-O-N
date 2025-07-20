// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeath);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChanged, float, NewHealt);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TRON_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Healt")
	float MaxHealth = 100.0f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Healt")
	float CurrentHealth;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stamina")
	float MaxStamina;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stamina")
	float CurrentStamina;
	
	UPROPERTY(EditDefaultsOnly, Category = "Armor")
	/** Should be a percentage between 0.0 and 1.0 */
	float MaxArmor = 1.0f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Armor")
	/** Should be a percentage between 0.0 and 1.0 */
	float BaseArmor = 0;
	
	UPROPERTY(EditDefaultsOnly, Category = "Armor")
	/** Should be a percentage between 0.0 and 1.0 */
	float CurrentArmor; // Riduce il danno preso di una percentuale

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintAssignable)
	FOnDeath OnDeath;

	UPROPERTY(BlueprintAssignable)
	FOnHealthChanged OnHealthChanged;
	
	UFUNCTION(BlueprintCallable)
	void TakeDamage(float Amount);
	
	UFUNCTION(BlueprintCallable)
	void InitializeStats(float defaultArmor, float mArmor, float mHealth, float mStamina);
	
	UFUNCTION(BlueprintCallable)
	float Heal(float Amount);
	
	UFUNCTION(BlueprintCallable)
	float GetCurrentHealth() const;

	UFUNCTION(BlueprintCallable)
	float GetMaxHealth() const;
	
	UFUNCTION(BlueprintCallable)
	float GetHealthPercent() const;
	
	UFUNCTION(BlueprintCallable)
	float ModifyCurrentArmor(float value);
	
	UFUNCTION(BlueprintCallable)
	float ResetCurrentArmor();
	
	UFUNCTION(BlueprintCallable)
	float GetCurrentArmor();
	
	UFUNCTION(BlueprintCallable)
	bool IsDead();
};
