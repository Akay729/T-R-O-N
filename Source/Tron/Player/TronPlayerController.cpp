// Fill out your copyright notice in the Description page of Project Settings.


#include "TronPlayerController.h"

#include "EnhancedInputComponent.h"
#include "Tron/Projectile/TronDiskProjectile.h"


// Sets default values
ATronPlayerController::ATronPlayerController()
{
}

void ATronPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	EnhancedInputComponent->BindAction(ThrowDiskAction, ETriggerEvent::Triggered, this, &ThisClass::ThrowDisk);
}

void ATronPlayerController::ThrowDisk()
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.Instigator = GetPawn();
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	AActor* NewActor = GetWorld()->SpawnActor<AActor>(DiskProjectile, GetPawn()->GetActorLocation(), GetPawn()->GetActorRotation(), SpawnParams);
	GetPawn()->MoveIgnoreActorAdd(NewActor);
}