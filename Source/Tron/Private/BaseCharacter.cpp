// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"
#include "Disk.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "InputActionValue.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Disk.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"
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
	
	if(DiskClass == nullptr) return;
	CharacterDisk = GetWorld()->SpawnActor<ADisk>(DiskClass);
	CharacterDisk->SetActorEnableCollision(false);
	
	if(CharacterDisk == nullptr) return;
	CharacterDisk->AttachToComponent(
		GetMesh(), 
		FAttachmentTransformRules::SnapToTargetNotIncludingScale, 
		TEXT("RightHandSocket"));
	CharacterDisk->SetOwner(this);

	UE_LOG(LogTemp, Warning, TEXT("CharacterDisk: %s"), *CharacterDisk->GetName());
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FVector ForwardDir = GetActorForwardVector();
	FVector start = GetActorLocation();
	DrawDebugLine(GetWorld(), start, start + ForwardDir * 500, FColor::Green, false, 0.1);

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
			Input->BindAction(IA_Jump, ETriggerEvent::Started, this, &ABaseCharacter::JumpAction);
			Input->BindAction(IA_ThrowDisk, ETriggerEvent::Started, this, &ABaseCharacter::ThrowDisk);
			Input->BindAction(IA_Dash, ETriggerEvent::Started, this, &ABaseCharacter::Dash);
			Input->BindAction(IA_Sprint, ETriggerEvent::Triggered, this, &ABaseCharacter::StartSprint);
			Input->BindAction(IA_Sprint, ETriggerEvent::Completed, this, &ABaseCharacter::StopSprint);
		}
	}

}

void ABaseCharacter::MoveAction(const FInputActionValue& value)
{
	FVector2D MoveVector = value.Get<FVector2D>();
	AddMovementInput(GetActorForwardVector(), MoveVector.Y);
	AddMovementInput(GetActorRightVector(), MoveVector.X);
}
void ABaseCharacter::LookAction(const FInputActionValue& value)
{
	FVector2D LookVector = value.Get<FVector2D>();
	AddControllerYawInput(LookVector.X);
	AddControllerPitchInput(LookVector.Y*-1);
	
}
void ABaseCharacter::JumpAction()
{
	//CharacterDisk->ReattachDiskToSocket();
	Jump();
}

void ABaseCharacter::ThrowDisk()
{
	CharacterDisk->Throw();

	/* FVector DiskSpawnLocation = DiskSpawnPoint->GetComponentLocation();
	FRotator DiskSpawnRotation = RootComponent->GetComponentRotation(); */
}

void ABaseCharacter::Dash()
{
	//const FVector ForwardDir = GetActorRotation().Vector();
	const FVector Direction = GetCharacterMovement()->GetLastInputVector();
	LaunchCharacter(Direction * DashDistance, true, true);

}

void ABaseCharacter::StartSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
}

void ABaseCharacter::StopSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}
