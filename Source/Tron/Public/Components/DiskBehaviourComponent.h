// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DiskBehaviourComponent.generated.h"

UENUM(BlueprintType)
enum class EDisk_State : uint8 {
	None	UMETA(DisplayName = "None"),
	Attached	UMETA(DisplayName = "Attached"),
	Throw	UMETA(DisplayName = "Thrown"),
	Returning	UMETA(DisplayName = "Returning"),
	Deflected	UMETA(DisplayName = "Deflected")
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TRON_API UDiskBehaviourComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDiskBehaviourComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void PerformMeleeAttack();
	void PerformRangedAttack();
	void PerformRecallDisk();
	void PerformBlock();
};
