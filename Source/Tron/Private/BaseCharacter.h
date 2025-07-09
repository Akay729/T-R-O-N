// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

class UInputMappingContext;
class UInputAction;
class ADisk;
class UHealthComponent;
struct FInputActionValue;

UCLASS()
class ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	//Input Action Function
	UFUNCTION()
	void MoveAction(const FInputActionValue& value);
	
	UFUNCTION()
	void LookAction(const FInputActionValue& value);
	
	UFUNCTION()
	void JumpAction();
	
	UFUNCTION()
	void ThrowDisk();
	
	UFUNCTION()
	void Dash();
	
	UFUNCTION()
	void StartSprint();
	
	UFUNCTION()
	void StopSprint();
	
	
	//Mapping Context to set in the BP class
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EnhancedInput")
	UInputMappingContext* DefaultMappingContext;

	//Input Action to set in the BP class
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EnhancedInput")
	UInputAction* IA_Move;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EnhancedInput")
	UInputAction* IA_Look;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EnhancedInput")
	UInputAction* IA_Jump;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EnhancedInput")
	UInputAction* IA_ThrowDisk;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EnhancedInput")
	UInputAction* IA_Dash;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EnhancedInput")
	UInputAction* IA_Sprint;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UHealthComponent* HealthComponent;

private:
	UPROPERTY(EditAnywhere, Category = "Components")
	USceneComponent* DiskSpawnPoint;


	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TSubclassOf<class ADisk> DiskClass;	

	ADisk* CharacterDisk;

	UPROPERTY(EditDefaultsOnly)
	float DashDistance = 500;

	UPROPERTY(EditDefaultsOnly, Category="Movement")
	float WalkSpeed = 350.f;

	UPROPERTY(EditDefaultsOnly, Category="Movement")
	float SprintSpeed = 650.f;

};
