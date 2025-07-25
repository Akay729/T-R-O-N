// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Disk.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetMathLibrary.h"
#include "BaseCharacter.h"
#include "Components/AttributeComponent.h"
#include "TimerManager.h"
#include "DataTypes/DamageTypes.h"
#include "Interfaces/Damagable.h"
#include "Components/MeleeWeaponComponent.h"
// Sets default values
#define ECC_DiskTrace ECollisionChannel::ECC_GameTraceChannel1

ADisk::ADisk()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DiskMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	RootComponent = DiskMeshComponent;

	MeleeWeaponComponent = CreateDefaultSubobject<UMeleeWeaponComponent>(TEXT("Test"));

	DiskMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement"));
	DiskMovementComponent->MaxSpeed = DiskMaxSpeed;
	DiskMovementComponent->InitialSpeed = 0;
	DiskMovementComponent->Velocity = FVector::ZeroVector;
	DiskMovementComponent->ProjectileGravityScale = 0.0f;
	DiskMovementComponent->OnProjectileBounce.AddDynamic(this, &ADisk::OnProjectileBounce);
}

// Called when the game starts or when spawned
void ADisk::BeginPlay()
{
	Super::BeginPlay();
	DiskMovementComponent->StopMovementImmediately();
	CurrentState = EDiskState::Attached;
	
	//Get Owner Info
	DiskOwner = GetOwner();
	OwnerController = GetOwnerController();
	DiskCharacterOwner = Cast<ABaseCharacter>(GetOwner());

	//Trace Params
	Params.AddIgnoredActor(DiskOwner);
	Params.AddIgnoredActor(this);

	//TEST ONLY
	MeleeDamageInfo.Amount = MeleeDamage;
	MeleeDamageInfo.CanBeBlocked = true;
	MeleeDamageInfo.CanBeParried = true;

	ThrowDamageInfo.Amount = ThrowDamage;
	ThrowDamageInfo.CanBeBlocked = true;
	ThrowDamageInfo.CanBeParried = false;
}

// Called every frame
void ADisk::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	DrawDebugSphere(GetWorld(), GetActorLocation(), DiskSphereRad, 36, FColor::Green, false, 0.1f);
}

int32 ADisk::GetDiskBounce()
{
	return BounceCount;
}

void ADisk::OnProjectileBounce(const FHitResult& ImpactResult, const FVector& ImpactVelocity)
{
    BounceCount++;
	
	AActor* HitActor = ImpactResult.GetActor();
	if (HitActor && this != HitActor && DiskOwner != HitActor)
	{
		if(HitActor->IsA(ABaseCharacter::StaticClass()))
		{
			GoBackToOwner();
			ApplayDamage(HitActor, ThrowDamageInfo);
		}
		//UE_LOG(LogTemp, Warning, TEXT("Target Valido"));
	}
	
	if (BounceCount >= MaxBounces)
	{	
		GoBackToOwner();
	}
}

void ADisk::ApplayDamage(AActor* TargetActor, FDamageInfo DamageInfo)
{
	if(TargetActor && TargetActor->GetClass()->ImplementsInterface(UDamagable::StaticClass()))
		{
			IDamagable::Execute_ReciveDamage(TargetActor, MeleeDamageInfo);
			UE_LOG(LogTemp, Warning, TEXT("Hit %s"), *TargetActor->GetName());
			//DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, MeleeAttackSphereRad, 16, FColor::Green, false, 2.f);
		}
}

void ADisk::Throw()
{
	if (DiskOwner == nullptr) return;
	if(OwnerController == nullptr) return;
	
	// Get player viewpoint location and rotation 
	FVector	ViewpointLocation;
	FRotator ViewpointRotation;
	OwnerController->GetPlayerViewPoint(ViewpointLocation, ViewpointRotation);
	
	if (CurrentState == EDiskState::Attached)
	{
		DiskSweepTraceForTaget(ViewpointLocation, ViewpointRotation);
		DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		
		DiskMovementComponent->StopMovementImmediately();
		DiskMovementComponent->Deactivate();
		DiskMovementComponent->SetUpdatedComponent(DiskMeshComponent); // ← importante
		DiskMovementComponent->SetComponentTickEnabled(true);
		DiskMeshComponent->IgnoreActorWhenMoving(GetOwner(), true);

		DiskMovementComponent->Activate(true);
		DiskMovementComponent->Velocity = FVector::ZeroVector; // reset prima di lanciare
		
		DiskMovementComponent->bShouldBounce = true;
		DiskMovementComponent->Bounciness = 1.f;
		DiskMovementComponent->Friction = 0.f;
		
		FVector LaunchDirection = ViewpointRotation.Vector();
		DiskMovementComponent->Velocity = LaunchDirection * DiskSpeed;
		SetActorRotation(LaunchDirection.Rotation());
		SetActorEnableCollision(true);
		
		CurrentState = EDiskState::Throw;
	}
}

//need rework
void ADisk::DiskSweepTraceForTaget(FVector ViewpointLocation, FRotator ViewpointRotation)
{
	FHitResult OutHit;
	FVector Start = ViewpointLocation;
	FVector End = Start + ViewpointRotation.Vector() * 1000;
	FCollisionShape SphereShape = FCollisionShape::MakeSphere(DiskSphereRad);
	//DrawDebugCapsule()
	
	bool isHit = GetWorld()->SweepSingleByChannel(OutHit, Start, End, FQuat::Identity, ECC_DiskTrace, SphereShape, Params);
	if (isHit)
	{
		AActor* HitActor = OutHit.GetActor();
		if (HitActor)
		{
			//ToDo
			//Definire il comportamento se l'actor è un player/nemico
			UE_LOG(LogTemp, Warning, TEXT("Attore colpito: %s"), *HitActor->GetName());
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Colpito qualcosa, ma nessun attore valido"));
		}
	}
}

void ADisk::GoBackToOwner()
{
	if (CurrentState == EDiskState::Throw)
	{
		if (DiskOwner == nullptr) return;

		DiskTrajectoryTimeHandle;
		DiskTimerDel.BindUObject(this, &ADisk::UpdateVelocityToActorLocation, DiskOwner);
		GetWorldTimerManager().SetTimer(
			DiskTrajectoryTimeHandle,
			DiskTimerDel,
			0.05f,
			true
		);
		
		CurrentState = EDiskState::Returning;
	}
}

void ADisk::UpdateVelocityToActorLocation(AActor* TargetActor)
{
	//Questa funzione si puo usare sia per il lancio che per il ritorno.

	if (!TargetActor || !DiskMovementComponent) return;

	FVector TargetActorLocation = TargetActor->GetActorLocation();
	FVector DiskLocation = GetActorLocation();

	DiskMovementComponent->bShouldBounce = false;
	//DiskMovementComponent->Velocity = FVector::ZeroVector;
	
	// Normalizza la direzione e scala per mantenere la stessa velocità
	FVector DirectionToTarget = (TargetActorLocation - DiskLocation).GetSafeNormal();
	DiskMovementComponent->Velocity = DirectionToTarget * DiskSpeed;
	ReattachDiskToSocket();
}

void ADisk::ReattachDiskToSocket()
{
	
	if (DiskCharacterOwner == nullptr) return;

	FVector SocketLocation = DiskCharacterOwner->GetMesh()->GetSocketLocation(TEXT("RightHandSocket"));
	if (FVector::Dist(GetActorLocation(), SocketLocation) < RangeToCatch)
	{
		GetWorldTimerManager().ClearTimer(DiskTrajectoryTimeHandle);
		bool isAttached = AttachToComponent(DiskCharacterOwner->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("RightHandSocket"));
		SetActorRotation(DiskCharacterOwner->GetMesh()->GetSocketRotation(TEXT("RightHandSocket")));

		SetActorEnableCollision(false);
		DiskMovementComponent->Velocity = FVector::ZeroVector;
		DiskMovementComponent->StopMovementImmediately();
		BounceCount = 0;
		
		if (isAttached)
		{
			CurrentState = EDiskState::Attached;
		}
	}
}

//MELEE
void ADisk::DoMeleeAttack(FVector StartPosition, FVector EndPosition)
{
	MeleeWeaponComponent->StartAttack();
/* 	if(CurrentState != EDiskState::Attached) return;
	
	FCollisionShape SphereShape = FCollisionShape::MakeSphere(MeleeAttackSphereRad);
	FHitResult HitResult;

	//Creare una sweep trace del disco
	bool isHit = GetWorld()->SweepSingleByChannel(
		HitResult, StartPosition, EndPosition, FQuat::Identity, ECC_DiskTrace, SphereShape, Params
	);
	
	DrawDebugCapsule(
		GetWorld(),
		(StartPosition + EndPosition) * 0.5f,
		SphereShape.GetSphereRadius(),       // half height
		SphereShape.GetSphereRadius(),       // radius
		FQuat::Identity,
		FColor::Red,
		false,     // persistent
		1.0f       // lifetime
	);

	if(isHit)
	{	
		AActor* HitActor = HitResult.GetActor();
		ApplayDamage(HitActor, MeleeDamageInfo);
	} */

}

//Utils
AController* ADisk::GetOwnerController()
{
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if(OwnerPawn) return OwnerPawn->GetController();
	return nullptr;
}


//TODO 
/*
- DiskStatedeve essere in comunicazione con ability component sarà neccesario fare una struc a parte per quello.
- Rework con componet (organizzar il tutto con una struttura modulare basata su component)

*/