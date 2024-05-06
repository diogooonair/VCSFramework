// Copyright 2023 Diogo Ventura, All Rights Reserved.


#include "AnimNotifies/VCS_RotateToAI_ANS.h"

#include "ActorComponents/VCS_AILibrary_AC.h"
#include "Kismet/GameplayStatics.h"

void UVCS_RotateToAI_ANS::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                      float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	const AActor* PlayerActor = MeshComp->GetOwner();
	APlayerController* PlayerController = Cast<APlayerController>(PlayerActor->GetInstigatorController());

	if(!PlayerController)return;
	AActor* CameraActor = PlayerController->GetViewTarget(); // Assuming the camera is the view target

	if (!PlayerActor || !CameraActor)
	{
		// Invalid input, return nullptr or handle accordingly
		return;
	}

	FVector CameraLocation = CameraActor->GetActorLocation();
	FVector PlayerLocation = PlayerActor->GetActorLocation();

	// Get all pawns in the world
	TArray<AActor*> AllActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APawn::StaticClass(), AllActors);

	APawn* ClosestPawn = nullptr;
	float ClosestDistanceSquared = MAX_FLT;

	for (AActor* CurrentActor : AllActors)
	{
		// Exclude the player actor
		if (CurrentActor != PlayerActor && CurrentActor->IsA<APawn>() && CurrentActor->FindComponentByClass<UVCS_AILibrary_AC>())
		{
			APawn* CurrentPawn = Cast<APawn>(CurrentActor);

			// Check if the pawn is within the camera's field of view
			FVector PawnLocation = CurrentPawn->GetActorLocation();
			FVector DirectionToPawn = (PawnLocation - CameraLocation).GetSafeNormal();
			FVector CameraForward = CameraActor->GetActorForwardVector();
			float DotProduct = FVector::DotProduct(DirectionToPawn, CameraForward);

			if (DotProduct > 0)
			{
				// Pawn is in front of the camera, check distance
				float DistanceSquared = FVector::DistSquared(PawnLocation, PlayerLocation);

				if (DistanceSquared < ClosestDistanceSquared)
				{
					// Found a closer pawn
					ClosestDistanceSquared = DistanceSquared;
					ClosestPawn = CurrentPawn;
				}
			}
		}
	}

	ActorToRotate = ClosestPawn;

	
}

void UVCS_RotateToAI_ANS::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	const AActor* PlayerActor = MeshComp->GetOwner();
	if(!ActorToRotate) return;
	
	const FVector DirectionToPawn = ActorToRotate->GetActorLocation() - PlayerActor->GetActorLocation();

	if (!DirectionToPawn.IsNearlyZero())
	{
		// Calculate the rotation angle in radians
		float RotationAngleRadians = FMath::Atan2(DirectionToPawn.Y, DirectionToPawn.X);

		// Convert radians to degrees
		float RotationAngleDegrees = FMath::RadiansToDegrees(RotationAngleRadians);

		// Only rotate around the Z-axis (yaw)
		FRotator TargetRotation = FRotator(0.0f, RotationAngleDegrees, 0.0f);

		// Interpolate smoothly towards the target rotation
		float InterpolationSpeed = 5.0f; // Adjust the speed as needed
		FRotator NewRotation = FMath::Lerp(PlayerActor->GetActorRotation(), TargetRotation, FrameDeltaTime * InterpolationSpeed);

		// Set the actor rotation
		PlayerActor->GetRootComponent()->SetWorldRotation(NewRotation);
	}
}
