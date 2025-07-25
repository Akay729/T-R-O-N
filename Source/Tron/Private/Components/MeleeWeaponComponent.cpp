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
    Owner = GetOwner();
    TopLevelOwner = Owner->GetOwner();
    OwnerController = GetOwnerController();
    
    
    Params.AddIgnoredActor(Owner);
    Params.AddIgnoredActor(TopLevelOwner);
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
    Attack();
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
    if (!Owner) return;
    if (!OwnerController) return;
    
    FVector Origin = TopLevelOwner->GetActorLocation();
    
    FVector OnwerLocation;
    FRotator OwnerRotation;

    OwnerController->GetPlayerViewPoint(OnwerLocation, OwnerRotation);

    FVector Forward = OwnerRotation.Vector();
    
    TSet<AActor*> AlreadyHit;

	SweepAttack(Origin, Forward, SphereRadius,AlreadyHit);
}
/**
 * @brief In questo gioco tutti gli attachi melee sono delgi sweep in arco di una capsula
 * 
 * @param Origin il centro per larcata dello sweep.
 * @param Forward la direzione in cui deve eseguire la arcata.
 * @param AlreadyHit Set of the actor hit by the sweeps
 * @return bool se ha colpito
 */
bool UMeleeWeaponComponent::SweepAttack( FVector Origin, FVector Forward, float Radius, TSet<AActor*>& ActorsHit)
{
	//Sweep per melee weapon
    TArray<FHitResult> Hits;
    
    FVector Start = Origin + FVector(0,0,50); //offset per fare partire il colpo dal centro corpo
    FVector End = Start + Forward * SweepDistance;
    FCollisionShape Sphere = FCollisionShape::MakeSphere(Radius);
    
    bool isHit = GetWorld()->SweepMultiByChannel(
        Hits,
        Start,
        End,
        FQuat::Identity,
        ECC_DiskTrace,
        Sphere,
        Params
    );

    // Debug: disegna la capsula e la linea di sweep
    DrawDebugSphere(GetWorld(), End, Radius, 16, FColor::Red, false, 1.0f);
    DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 1.0f);

    if(isHit)
    {
        for (const FHitResult& Hit : Hits)
        {
            AActor* HittedActor = Hit.GetActor();
            if (HittedActor && !ActorsHit.Contains(HittedActor))
            {
                ActorsHit.Add(HittedActor);
                UE_LOG(LogTemp, Warning, TEXT("Colpito: %s"), *HittedActor->GetName());
                
                //Fare Danno
            }
        }
    }
    return isHit;
}

AController* UMeleeWeaponComponent::GetOwnerController()
{
	APawn* OwnerPawn = Cast<APawn>(Owner->GetOwner());
	if(OwnerPawn) return OwnerPawn->GetController();
	return nullptr;
}