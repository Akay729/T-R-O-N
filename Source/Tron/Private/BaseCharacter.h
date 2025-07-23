// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/Damagable.h"
#include "GameFramework/Character.h"
#include "DataTypes/CharacterStateTypes.h"
#include "BaseCharacter.generated.h"

class UInputMappingContext;
class UInputAction;
class ADisk;
class UAttributeComponent;
class UCombatComponent;
struct FInputActionValue;

UCLASS()
class ABaseCharacter : public ACharacter, public IDamagable
{
	GENERATED_BODY()

public:
	// Constructor
	ABaseCharacter();

	// Overrides
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Input Actions
	UFUNCTION(BlueprintCallable)
	void MoveAction(const FInputActionValue& value);

	UFUNCTION(BlueprintCallable)
	void LookAction(const FInputActionValue& value);

	UFUNCTION(BlueprintCallable)
	void JumpAction();

	UFUNCTION(BlueprintCallable)
	void ThrowDisk();

	UFUNCTION(BlueprintCallable)
	void Dash();

	UFUNCTION()
	void StartSprint();

	UFUNCTION()
	void StopSprint();
	
	//Combat
	UFUNCTION(BlueprintCallable)
	void MeleeAttack();

	UFUNCTION(BlueprintCallable)
	void StartBlock();
	
	UFUNCTION(BlueprintCallable)
	void StopBlock();

	UFUNCTION(BlueprintCallable)
	void StartParryWindow();
	
	UFUNCTION()
	void EndParryWindow();
	
	// Dashing
	UFUNCTION(BlueprintCallable)
	bool IsDashing() const { return bIsDashing; }

	void StartDashing();
	void StopDashing();

	// Damagable Interface Implementation
	float GetCurrentHealth_Implementation();
	float GetMaxHealth_Implementation();
	float Heal_Implementation(float Amount);
	bool ReciveDamage_Implementation(FDamageInfo DamageInfo);

	UFUNCTION(BlueprintCallable)
	EMovementState GetCurrentMovementState() const;
	
	UFUNCTION(BlueprintCallable)
	EAttackState GetCurrentAttackState() const;
	
protected:
	// BeginPlay
	virtual void BeginPlay() override;

	// Movement flags
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	bool bIsDashing;

	// Attribute Component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UAttributeComponent* AttributeComponent;
	
	// Combat Component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCombatComponent* CombatComponent;
	
	// Input Mapping & Actions
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EnhancedInput")
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EnhancedInput")
	UInputAction* IA_Move;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EnhancedInput")
	UInputAction* IA_Look;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EnhancedInput")
	UInputAction* IA_Jump;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EnhancedInput")
	UInputAction* IA_Dash;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EnhancedInput")
	UInputAction* IA_Sprint;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EnhancedInput")
	UInputAction* IA_ThrowDisk;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EnhancedInput")
	UInputAction* IA_MeleeAttack;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EnhancedInput")
	UInputAction* IA_Block_and_Parry;

private:
	void SetAttackStateToNone();
	EAttackState CurrentAttackState;
	EMovementState CurrentMovementState;
	// Components
	UPROPERTY(EditAnywhere, Category = "Components")
	USceneComponent* DiskSpawnPoint;

	// Disk setup
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TSubclassOf<class ADisk> DiskClass;

	ADisk* CharacterDisk;

	// Movement values
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float DashDistance = 500.f;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float WalkSpeed = 350.f;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float SprintSpeed = 650.f;

	////////// ----------- Charachert Stats -----------//////////
	UPROPERTY(EditDefaultsOnly, Category = "Character Stat")
	float BaseArmor = 0.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Character Stat")
	float MaxArmor = 1.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Character Stat")
	float MaxHealth = 1000.0f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Character Stat")
	float MaxStamina = 100.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Character Stat")
	float DashArmor = 0.50f;

	UPROPERTY(EditDefaultsOnly, Category = "Character Stat")
	float MeleeAttackRange = 200.0f;

	UPROPERTY(EditDefaultsOnly, Category="Character Stat")
	float ParryWindowTime = 0.5f;

	bool bIsBlocking = false;
	bool bIsParrying = false;
	
	////////// ----------- Timer handles -----------//////////
	
	// Dash
	FTimerHandle DashTimerHandle;
	// Parry
	FTimerHandle ParryTimerHandle;
	//Thorw to None state
	FTimerHandle ThrowTimerHandle;
};