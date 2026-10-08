// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TronPlayerCharacter.generated.h"

struct FInputActionValue;
class UInputAction;
class UInputMappingContext;
class USpringArmComponent;
class UCameraComponent;

UCLASS()
class TRON_API ATronPlayerCharacter : public ACharacter
{
	GENERATED_BODY()

protected:
	//Components
	
	UPROPERTY(VisibleAnywhere, Category=Components)
	TObjectPtr<UCameraComponent> CameraComponent;
	
	UPROPERTY(VisibleAnywhere, Category=Components)
	TObjectPtr<USpringArmComponent> SpringArmComponent;

	//IA and IMC
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Components)
	TObjectPtr<UInputMappingContext> DefaultMappingContext;
	
	TObjectPtr<UInputAction> IA_Move;
	TObjectPtr<UInputAction> IA_Look;
	
	
	//Functions
	void MoveAction(const FInputActionValue& Value);
	void LookAction(const FInputActionValue& Value);
	
	
public:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	ATronPlayerCharacter();
};
