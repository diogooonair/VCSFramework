// Copyright 2023 Diogo Ventura, All Rights Reserved.


#include "ActorComponents/VCS_CombatComponent_AC.h"
#include "Engine/World.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "TimerManager.h"
#include "Engine/EngineTypes.h"
#include "ActorComponents/VCS_AILibrary_AC.h"
#include "ActorComponents/VCS_StateManagerComponent_AC.h"
#include "Kismet/KismetMathLibrary.h"
#include "PlayerBases/VCS_BaseWeapon_Actor.h"
#include "Animation/AnimMontage.h"
#include "GameFramework/PlayerController.h"
#include "PlayerBases/VCS_CharacterBase.h"

// Sets default values for this component's properties
UVCS_CombatComponent_AC::UVCS_CombatComponent_AC() : TargetActor(nullptr),
                                                     PlayerStateComponent(nullptr), Weapon(nullptr),
                                                     TargetInput(nullptr)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	Health = DefaultHealth, Stamina = DefaultStamina,Armor = DefaultArmor;
}


// Called when the game starts
void UVCS_CombatComponent_AC::BeginPlay()
{
	Super::BeginPlay();

	PlayerStateComponent = Cast<UVCS_StateManagerComponent_AC>(GetOwner()->GetComponentByClass(UVCS_StateManagerComponent_AC::StaticClass()));

	FTimerHandle DelayTimer;
	GetWorld()->GetTimerManager().SetTimer(
		DelayTimer, this, &UVCS_CombatComponent_AC::SetupVariables, 0.1f, false);
}

void UVCS_CombatComponent_AC::SetupVariables()
{
	APlayerController* PC = Cast<APlayerController>(GetOwner()->GetInstigatorController());
	if(PC){
		UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PC->GetComponentByClass(UEnhancedInputComponent::StaticClass()));

		if(Input){
			//Loop through all setup actions and start tracking the input
			//Adding extra input check because unreal did one of those things where it crashes because it feels like it

			if(TargetInput && Input)
			{
				Input->BindAction(TargetInput, ETriggerEvent::Completed, this, &UVCS_CombatComponent_AC::TargetActorInput);
			}
		}
	}

	Health = DefaultHealth;
	Armor = DefaultArmor;
	Stamina = DefaultStamina;
}


void UVCS_CombatComponent_AC::TargetActorInput(const FInputActionValue& Value)
{
	if(!bShouldTarget)
	{
		bShouldTarget = true;
		return;
	}
	if(!PlayerStateComponent->CanPerformAction(TargetInput, ""))return;

	if(TargetActor || TargetActor != nullptr) UnTarget();
	
	// Get the player's camera manager
	APlayerController* PlayerController = Cast<APlayerController>(GetOwner()->GetInstigatorController());
	APlayerCameraManager* CameraManager = PlayerController->PlayerCameraManager;

	// Get the camera location
	FVector CameraLocation = CameraManager->GetCameraLocation();

	// Create a sphere to overlap actors within the specified range
	FCollisionShape CollisionSphere = FCollisionShape::MakeSphere(CameraLockRange);

	// Ignore the character itself in the overlap check
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(GetOwner());

	// Array to store overlapping actors
	TArray<FHitResult> HitResults;

	// Perform the sphere overlap check
	bool bOverlapResult = GetWorld()->SweepMultiByChannel(
		HitResults,
		CameraLocation,
		CameraLocation,
		FQuat::Identity,
		ECC_Pawn,
		CollisionSphere,
		CollisionParams
	);

	if(!bOverlapResult){return;}

	// Initialize variables for tracking the closest actor
	AActor* ClosestActor = nullptr;
	float ClosestDistanceSquared = MAX_FLT;

	// Iterate through the overlapping actors and find the closest one to the camera center
	for (const FHitResult& HitResult : HitResults)
	{
		AActor* CurrentActor = HitResult.GetActor();
		if (CurrentActor && CurrentActor->FindComponentByClass(UVCS_AILibrary_AC::StaticClass()))
		{
			FVector ActorLocation = CurrentActor->GetActorLocation();
			FVector CameraToActor = ActorLocation - CameraLocation;
			float DistanceSquared = CameraToActor.SizeSquared();

			if (DistanceSquared < ClosestDistanceSquared)
			{
				ClosestActor = CurrentActor;
				ClosestDistanceSquared = DistanceSquared;
			}
		}
	}


	if(ClosestActor)
	{
		Target(ClosestActor);
	}
}
// Called every frame
void UVCS_CombatComponent_AC::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(!PlayerStateComponent)return;

	if(!PlayerStateComponent->CanPerformAction(TargetInput, "")) {TargetActor = nullptr;return;}

	if(!TargetActor || TargetActor == nullptr)return;

	FRotator ControlRotation = GetOwner()->GetInstigatorController()->GetControlRotation();
	FRotator rotation = UKismetMathLibrary::FindLookAtRotation(GetOwner()->GetActorLocation(), TargetActor->GetActorLocation());
	GetOwner()->GetInstigatorController()->SetControlRotation(FRotator(ControlRotation.Pitch,FMath::RInterpTo(ControlRotation, rotation, GetWorld()->GetDeltaSeconds(), 10.f).Yaw , ControlRotation.Roll));
}

float UVCS_CombatComponent_AC::GetHealth()
{
	return Health;
}

float UVCS_CombatComponent_AC::GetArmor()
{
	return Armor;
}

float UVCS_CombatComponent_AC::GetStamina()
{
	return Stamina;
}



float UVCS_CombatComponent_AC::TakeHealth(float Damage, bool bCountArmor)
{
	if(bCountArmor)
	{
		const float DamageReduction = Armor / (Armor + 100);
		const float ReducedDamage = Damage * (1 - DamageReduction);
		Damage = ReducedDamage;
	}

	Health -= Damage;
	if(Health < 0.f)Health = 0.f;

	
	return Health;
}

void UVCS_CombatComponent_AC::Target(AActor* actor)
{
	TargetActor = actor;
}

void UVCS_CombatComponent_AC::UnTarget()
{
	TargetActor = nullptr;
}

void UVCS_CombatComponent_AC::OnMontageEnd()
{
	ResetState();
}

void UVCS_CombatComponent_AC::PerformAttack(FName AttackingState, UAnimMontage* Montage, float montagespeed)
{
	if(PlayerStateComponent->IsStateValid(AttackingState))
	{
		LastState = PlayerStateComponent->CurrentState;
		PlayerStateComponent->CurrentState = AttackingState;
	}


	Cast<AVCS_CharacterBase>(GetOwner())->PlayAnimMontage(Montage, montagespeed);
	FTimerHandle timer;
	GetWorld()->GetTimerManager().SetTimer(
	timer, 
	this, // the owning object
	&UVCS_CombatComponent_AC::OnMontageEnd, // function to call on elapsed
	Montage->GetPlayLength() * montagespeed, // float delay until elapsed
	false);
}

void UVCS_CombatComponent_AC::ResetState()
{
	if(PlayerStateComponent->IsStateValid(LastState))
	{
		PlayerStateComponent->CurrentState = LastState;
	}
}

ACharacter* UVCS_CombatComponent_AC::GetTargetOrPossible()
{
	if(TargetActor)return Cast<ACharacter>(TargetActor);
	
	// Get the player's camera manager
	APlayerController* PlayerController = Cast<APlayerController>(GetOwner()->GetInstigatorController());
	APlayerCameraManager* CameraManager = PlayerController->PlayerCameraManager;

	// Get the camera location
	FVector CameraLocation = CameraManager->GetCameraLocation();

	// Create a sphere to overlap actors within the specified range
	FCollisionShape CollisionSphere = FCollisionShape::MakeSphere(CameraLockRange);

	// Ignore the character itself in the overlap check
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(GetOwner());

	// Array to store overlapping actors
	TArray<FHitResult> HitResults;

	// Perform the sphere overlap check
	bool bOverlapResult = GetWorld()->SweepMultiByChannel(
		HitResults,
		CameraLocation,
		CameraLocation,
		FQuat::Identity,
		ECC_Pawn,
		CollisionSphere,
		CollisionParams
	);

	if(!bOverlapResult) return nullptr;
	
	// Initialize variables for tracking the closest actor
	AActor* ClosestActor = nullptr;
	float ClosestDistanceSquared = MAX_FLT;


	
	// Iterate through the overlapping actors and find the closest one to the camera center
	for (const FHitResult& HitResult : HitResults)
	{
		AActor* CurrentActor = HitResult.GetActor();
		if (CurrentActor && CurrentActor->FindComponentByClass(UVCS_AILibrary_AC::StaticClass()))
		{
			FVector ActorLocation = CurrentActor->GetActorLocation();
			FVector CameraToActor = ActorLocation - CameraLocation;
			float DistanceSquared = CameraToActor.SizeSquared();

			if (DistanceSquared < ClosestDistanceSquared)
			{
				ClosestActor = CurrentActor;
				ClosestDistanceSquared = DistanceSquared;
			}
		}
	}

	return Cast<ACharacter>(ClosestActor);
	
}

ACharacter* UVCS_CombatComponent_AC::GetClosestActorToLeftOrRightOfTarget(bool bToLeft)
{
    if (!TargetActor)
        return nullptr;

    // Get the player's camera manager
    APlayerController* PlayerController = Cast<APlayerController>(GetOwner()->GetInstigatorController());
    APlayerCameraManager* CameraManager = PlayerController->PlayerCameraManager;

    // Get the camera location and forward vector
    FVector CameraLocation = CameraManager->GetCameraLocation();
    FVector CameraForward = CameraManager->GetCameraRotation().Vector();

    // Get the target actor's location
    FVector TargetLocation = TargetActor->GetActorLocation();

    // Create a sphere to overlap actors within the specified range
    FCollisionShape CollisionSphere = FCollisionShape::MakeSphere(CameraLockRange);

    // Ignore the character itself in the overlap check
    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(GetOwner());

    // Array to store overlapping actors
    TArray<FHitResult> HitResults;

    // Perform the sphere overlap check
    bool bOverlapResult = GetWorld()->SweepMultiByChannel(
        HitResults,
        CameraLocation,
        CameraLocation,
        FQuat::Identity,
        ECC_Pawn,
        CollisionSphere,
        CollisionParams
    );

    if (!bOverlapResult)
        return nullptr;

    // Initialize variables for tracking the closest actor
    AActor* ClosestActor = nullptr;
    float ClosestDistanceSquared = MAX_FLT;

    // Iterate through the overlapping actors and find the closest one to the left or right of the target actor
    for (const FHitResult& HitResult : HitResults)
    {
        AActor* CurrentActor = HitResult.GetActor();
        if (CurrentActor && CurrentActor->FindComponentByClass(UVCS_AILibrary_AC::StaticClass()) && CurrentActor != TargetActor)
        {
            FVector ActorLocation = CurrentActor->GetActorLocation();
            FVector CameraToActor = ActorLocation - CameraLocation;

            // Calculate angle between camera forward vector and vector to actor
            float AngleToTarget = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(CameraToActor.GetSafeNormal(), CameraForward)));

            // Check if the actor is on the left or right of the target actor based on the sign of the cross product
            bool bIsToLeftOfTarget = FVector::CrossProduct(CameraToActor, TargetLocation - ActorLocation).Z > 0;

            if ((bToLeft && bIsToLeftOfTarget) || (!bToLeft && !bIsToLeftOfTarget))
            {
                float DistanceSquared = CameraToActor.SizeSquared();
                if (DistanceSquared < ClosestDistanceSquared)
                {
                    ClosestActor = CurrentActor;
                    ClosestDistanceSquared = DistanceSquared;
                }
            }
        }
    }

    return Cast<ACharacter>(ClosestActor);
}


void UVCS_CombatComponent_AC::PerformFinisher(ACharacter* Killer, ACharacter* Victim, UAnimMontage* KillerMontage, UAnimMontage* VictimMontage,FName KillerState, FName VictimState, float DesiredDistance)
{
	if (!Killer || !Victim)
	{
		// Ensure that both the killer and the victim are valid characters
		return;
	}

	Killer->FindComponentByClass<UVCS_StateManagerComponent_AC>()->CurrentState = KillerState;
	Victim->FindComponentByClass<UVCS_StateManagerComponent_AC>()->CurrentState = VictimState;

	// Calculate the direction between the killer and the victim
	FVector Direction = Victim->GetActorLocation() - Killer->GetActorLocation();
	Direction.Normalize();

	// Calculate the new positions for the killer and the victim
	FVector KillerNewLocation = Victim->GetActorLocation() - (Direction * DesiredDistance);
	FVector VictimNewLocation = Killer->GetActorLocation() + (Direction * DesiredDistance);

	// Set the new positions for the characters
	Killer->SetActorLocation(KillerNewLocation);
	Victim->SetActorLocation(VictimNewLocation);
	

	// Play the killer's anim montage
	if (Killer->GetMesh())
	{
		Killer->PlayAnimMontage(KillerMontage);
	}

	// Play the victim's anim montage
	if (Victim->GetMesh())
	{
		Victim->PlayAnimMontage(VictimMontage);
	}
}

AActor* UVCS_CombatComponent_AC::GetTargetActor()
{
	return TargetActor;
}



