// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/DiskBehaviourComponent.h"
#include "Components/MeleeWeaponComponent.h"
#include "Components/RangedWeaponComponent.h"

// Sets default values for this component's properties
UDiskBehaviourComponent::UDiskBehaviourComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UDiskBehaviourComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UDiskBehaviourComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UDiskBehaviourComponent::PerformMeleeAttack()
{
	
}
void UDiskBehaviourComponent::PerformRangedAttack()
{

}
void UDiskBehaviourComponent::PerformRecallDisk()
{

}
void UDiskBehaviourComponent::PerformBlock()
{

}

