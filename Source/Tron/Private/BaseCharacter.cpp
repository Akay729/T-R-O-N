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
#include "Components/AttributeComponent.h"
#include "Components/CombatComponent.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AttributeComponent = CreateDefaultSubobject<UAttributeComponent>(TEXT("Health Component"));
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

	if (AttributeComponent)
	{
		AttributeComponent->InitializeStats(BaseArmor, MaxArmor, MaxHealth, MaxStamina);
		UE_LOG(LogTemp, Warning, TEXT("HP: %f"), AttributeComponent->GetHealthPercent()*100.f);
	}
	
	//Disk Creation
	if(!DiskClass) return;
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;

	CharacterDisk = GetWorld()->SpawnActor<ADisk>(DiskClass, SpawnParams);

	//Attach Disk to mesh socket
	if(!CharacterDisk) return;
	CharacterDisk->SetActorEnableCollision(false);
	CharacterDisk->AttachToComponent(
		GetMesh(), 
		FAttachmentTransformRules::SnapToTargetNotIncludingScale, 
		TEXT("RightHandSocket"));
	//CharacterDisk->SetOwner(this);

	//UE_LOG(LogTemp, Warning, TEXT("CharacterDisk: %s"), *CharacterDisk->GetName());
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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
			Input->BindAction(IA_MeleeAttack, ETriggerEvent::Started, this, &ABaseCharacter::MeleeAttack);
			Input->BindAction(IA_Dash, ETriggerEvent::Started, this, &ABaseCharacter::Dash);
			Input->BindAction(IA_Sprint, ETriggerEvent::Triggered, this, &ABaseCharacter::StartSprint);
			Input->BindAction(IA_Sprint, ETriggerEvent::Completed, this, &ABaseCharacter::StopSprint);
			Input->BindAction(IA_Block_and_Parry, ETriggerEvent::Started, this, &ABaseCharacter::StartParryWindow);
			Input->BindAction(IA_Block_and_Parry, ETriggerEvent::Triggered, this, &ABaseCharacter::StartBlock);
			Input->BindAction(IA_Block_and_Parry, ETriggerEvent::Completed, this, &ABaseCharacter::StopBlock);
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

////////// ----------- COMBAT -----------//////////
void ABaseCharacter::MeleeAttack()
{	
	FVector Start = GetActorLocation();
	FVector End = Start + GetActorForwardVector() *  MeleeAttackRange;
	CharacterDisk->DoMeleeAttack(Start, End);
}

void ABaseCharacter::StartBlock()
{
	bIsBlocking = true;
	CombatComponent->SetIsBlocking(bIsBlocking);
}

void ABaseCharacter::StopBlock()
{
	CombatComponent->SetIsBlocking(false);
}

void ABaseCharacter::StartParryWindow()
{
	bIsParrying = true;
	GetWorld()->GetTimerManager().SetTimer(
		ParryTimerHandle, 
		this, 
		&ABaseCharacter::EndParryWindow, 
		ParryWindowTime, 
		false
	);
	CombatComponent->SetIsParring(bIsParrying);
}

void ABaseCharacter::EndParryWindow()
{
	bIsParrying = false;
	CombatComponent->SetIsParring(bIsParrying);
}

void ABaseCharacter::ThrowDisk()
{
	if (CharacterDisk) CharacterDisk->Throw();
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
	AttributeComponent->ModifyCurrentArmor(DashArmor);

	float CurrentArmor = AttributeComponent->GetCurrentArmor();
	UE_LOG(LogTemp, Warning, TEXT("Armor: %f"), CurrentArmor);
}
void ABaseCharacter::StopDashing()
{
	bIsDashing = false;
	AttributeComponent->ModifyCurrentArmor(-DashArmor);

	float CurrentArmor = AttributeComponent->GetCurrentArmor();
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
float ABaseCharacter::GetCurrentHealth_Implementation()
{
	return AttributeComponent->GetCurrentHealth();
}
float ABaseCharacter::GetMaxHealth_Implementation()
{
	return AttributeComponent->GetMaxHealth();
} 
float ABaseCharacter::Heal_Implementation(float Amount)
{
	return AttributeComponent->Heal(Amount);
} 
bool ABaseCharacter::ReciveDamage_Implementation(FDamageInfo DamageInfo)
{
	bool WasDamage;
	CombatComponent->ReciveDamage(DamageInfo, WasDamage);
	return WasDamage;
}