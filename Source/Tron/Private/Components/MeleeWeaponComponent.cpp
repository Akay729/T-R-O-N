// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/MeleeWeaponComponent.h"
//#include "MeleeWeaponComponent.h"
#include "Kismet/KismetSystemLibrary.h"

#define ECC_DiskTrace ECollisionChannel::ECC_GameTraceChannel1

// Sets default values for this component's properties
UMeleeWeaponComponent::UMeleeWeaponComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMeleeWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UMeleeWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UMeleeWeaponComponent::StartAttack()
{

}
void UMeleeWeaponComponent::StopAttack()
{

}
EAttacksState UMeleeWeaponComponent::GetCurrentAttackState()
{
    return Super::GetCurrentAttackState();
}

void UMeleeWeaponComponent::Attack()
{
	
}
void UMeleeWeaponComponent::SweepAttack()
{
	//Sweep per melee weapon
	// Crea una capsule davanti a se 
	// Identifica gli attori colpiti
  AActor* Owner = GetOwner();
    if (!Owner) return;

    FVector Origin = Owner->GetActorLocation();
    FVector Forward = Owner->GetActorForwardVector();
    
    float Radius = 30.f;
    float HalfHeight = 60.f;
    float SweepDistance = 200.f;

    int NumTraces = 8;
    float ArcAngle = 90.f; // angolo totale dell’arco (gradi)
    float StartAngle = -ArcAngle / 2.f;

    TSet<AActor*> AlreadyHit;
    
    // Rotazione per orientare la capsula orizzontalmente
    FQuat CapsuleRotation = FQuat(FVector::RightVector, FMath::DegreesToRadians(90.f)); // 90° su asse Y

    for (int i = 0; i < NumTraces; ++i)
    {
        float AngleDeg = StartAngle + (ArcAngle / (NumTraces - 1)) * i;
        FVector Direction = Forward.RotateAngleAxis(AngleDeg, FVector::UpVector);

        FVector Start = Origin + FVector(0, 0, 50.f); // leggero offset verticale
        FVector End = Start + Direction * SweepDistance;

        TArray<FHitResult> Hits;
        FCollisionShape Capsule = FCollisionShape::MakeCapsule(Radius, HalfHeight);

        bool bHit = GetWorld()->SweepMultiByChannel(
            Hits,
            Start,
            End,
            CapsuleRotation,
            ECC_Pawn,
            Capsule,
            FCollisionQueryParams(FName(), false, Owner)
        );

        // Debug: disegna la capsula e la linea di sweep
        DrawDebugCapsule(GetWorld(), End, HalfHeight, Radius, CapsuleRotation, FColor::Cyan, false, 1.0f);
        DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 1.0f);

        if (bHit)
        {
            for (const FHitResult& Hit : Hits)
            {
                AActor* HitActor = Hit.GetActor();
                if (HitActor && !AlreadyHit.Contains(HitActor))
                {
                    AlreadyHit.Add(HitActor);
                    UE_LOG(LogTemp, Warning, TEXT("Colpito: %s"), *HitActor->GetName());

                    // Qui puoi applicare danno o effetti al bersaglio
                }
            }
        }
    }
}

