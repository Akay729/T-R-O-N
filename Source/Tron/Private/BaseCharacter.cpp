// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"
#include "Disk.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "InputActionValue.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DiskSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Disk Throw SpawnPoint"));
	DiskSpawnPoint->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem = 
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());

		Subsystem->AddMappingContext(DefaultMappingContext, 0);

		if (UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent))
		{
			Input->BindAction(IA_Move, ETriggerEvent::Triggered, this, &ABaseCharacter::MoveAction);
			Input->BindAction(IA_Look, ETriggerEvent::Triggered, this, &ABaseCharacter::LookAction);
			Input->BindAction(IA_Jump, ETriggerEvent::Triggered, this, &ABaseCharacter::Jump);
			Input->BindAction(IA_ThrowDisk, ETriggerEvent::Triggered, this, &ABaseCharacter::ThrowDisk);
		}
	}

}

void ABaseCharacter::MoveAction(const FInputActionValue& value)
{
	UE_LOG(LogTemp, Warning, TEXT("Moving"));
	FVector2D MoveVector = value.Get<FVector2D>();
	AddMovementInput(GetActorForwardVector(), MoveVector.Y);
	AddMovementInput(GetActorRightVector(), MoveVector.X);
}
void ABaseCharacter::LookAction(const FInputActionValue& value)
{
	UE_LOG(LogTemp, Warning, TEXT("Looking"));
	FVector2D LookVector = value.Get<FVector2D>();
	AddControllerYawInput(LookVector.X);
	AddControllerPitchInput(LookVector.Y*-1);
	
}
void ABaseCharacter::JumpAction()
{
	UE_LOG(LogTemp, Warning, TEXT("Jumping"));
	Jump();
}

void ABaseCharacter::ThrowDisk()
{
	UE_LOG(LogTemp, Warning, TEXT("Throwing"));
}
