// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "InputActionValue.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Items/Disk.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Components/HealthComponent.h"
#include "Components/CombatComponent.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health Component"));
	CombatComponent = CreateDefaultSubobject<UCombatComponent>(TEXT("Combat Component"));

	//NOT USED YET
	DiskSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Disk SpawnPoint Component"));
	DiskSpawnPoint->SetupAttachment(RootComponent);
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (HealthComponent)
	{
		HealthComponent->InitializeStats(BaseArmor, MaxArmor, MaxHealth);
		UE_LOG(LogTemp, Warning, TEXT("HP: %f"), HealthComponent->GetHealthPercent()*100.f);
	}
	
	//Disk Creation
	if(!DiskClass) return;
	CharacterDisk = GetWorld()->SpawnActor<ADisk>(DiskClass);

	//Attach Disk to mesh socket
	if(!CharacterDisk) return;
	CharacterDisk->SetActorEnableCollision(false);
	CharacterDisk->AttachToComponent(
		GetMesh(), 
		FAttachmentTransformRules::SnapToTargetNotIncludingScale, 
		TEXT("RightHandSocket"));
	CharacterDisk->SetOwner(this);

	//UE_LOG(LogTemp, Warning, TEXT("CharacterDisk: %s"), *CharacterDisk->GetName());
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

/* 	FVector ForwardDir = GetActorForwardVector();
	FVector start = GetActorLocation(); */
}

// Called to bind functionality to input 
// Need Refactor to make it modular with costum commands
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

////////// ----------- MOVEMENT AND CAMERA MOVEMENT -----------//////////
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

////////// ----------- JUMP -----------//////////
void ABaseCharacter::JumpAction()
{
	//TO DO
	Jump();
}

////////// ----------- DASH METHOD -----------//////////
void ABaseCharacter::Dash()
{
	if(!IsDashing())
	{
		StartDashing();
		const FVector Direction = GetCharacterMovement()->GetLastInputVector();
		LaunchCharacter(Direction * DashDistance, true, true);
		
		GetWorldTimerManager().SetTimer(DashTimerHandle, this, &ABaseCharacter::StopDashing, 0.5f);
	}
}
void ABaseCharacter::StartDashing()
{
	bIsDashing = true;
	HealthComponent->ModifyCurrentArmor(DashArmor);

	float CurrentArmor = HealthComponent->GetCurrentArmor();
	UE_LOG(LogTemp, Warning, TEXT("Armor: %f"), CurrentArmor);
}
void ABaseCharacter::StopDashing()
{
	bIsDashing = false;
	HealthComponent->ModifyCurrentArmor(-DashArmor);

	float CurrentArmor = HealthComponent->GetCurrentArmor();
	UE_LOG(LogTemp, Warning, TEXT("Armor: %f"), CurrentArmor);
}

////////// ----------- SPRINT METHOD -----------//////////
void ABaseCharacter::StartSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
}
void ABaseCharacter::StopSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

////////// ----------- COMBAT SYSTEM METHOD -----------//////////
void ABaseCharacter::ThrowDisk()
{
	if (CharacterDisk) CharacterDisk->Throw();
}
float ABaseCharacter::GetCurrentHealth()
{
	return HealthComponent->GetCurrentHealth();
}
float ABaseCharacter::GetMaxHealth()
{
	return HealthComponent->GetMaxHealth();
} 
float ABaseCharacter::Heal(float Amount)
{
	return HealthComponent->Heal(Amount);
} 
bool ABaseCharacter::TakeDamage(FDamageInfo DamageInfo)
{
	bool WasDamage;
	CombatComponent->ReciveDamage(DamageInfo, WasDamage);
	return WasDamage;
}