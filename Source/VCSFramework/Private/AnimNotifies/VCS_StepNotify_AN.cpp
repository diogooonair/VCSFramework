// Copyright 2023 Diogo Ventura, All Rights Reserved.


#include "AnimNotifies/VCS_StepNotify_AN.h"

#include "ActorComponents/VCS_MovementComponent_AC.h"
#include "Datatables/VCS_StepDataTable_DT.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "NiagaraFunctionLibrary.h"
#include "Sound/SoundCue.h"

void UVCS_StepNotify_AN::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	AActor* Player = MeshComp->GetOwner();
	if(!Player)
		return;
	
	const FVector PlayerLocation = Player->GetActorLocation();
	const FVector TraceStart = PlayerLocation;
	const FVector TraceEnd = PlayerLocation + Player->GetActorUpVector() * -150.f;
	
	FHitResult HitResult; // Variable to store hit result

	// Parameters for the trace
	TArray<AActor*> ActorsToIgnore;

	

	// Perform the line trace
	bool bHit = UKismetSystemLibrary::LineTraceSingle(
		MeshComp,
		TraceStart,
		TraceEnd,
		UEngineTypes::ConvertToTraceType(ECC_Visibility),
		false,
		ActorsToIgnore,
		EDrawDebugTrace::None,
		HitResult,
		true, FLinearColor::Black
	);
	

	if (bHit)
	{
		// Handle the hit result as needed
		const UPhysicalMaterial* PhysicalMaterial = HitResult.PhysMaterial.Get();
		UVCS_MovementComponent_AC* movementComponent = Cast<UVCS_MovementComponent_AC>(Player->GetComponentByClass(UVCS_MovementComponent_AC::StaticClass()));
		if(!movementComponent)return;
		TArray<FStepDataStruct*> StepsDataArray;
		movementComponent->StepsData->GetAllRows<FStepDataStruct>("", StepsDataArray);
		for (const FStepDataStruct* StepsData : StepsDataArray)
		{
			if(StepsData->PhysicalMaterial->SurfaceType == PhysicalMaterial->SurfaceType)
			{
				//Debug::Print(PhysicalMaterial->GetName());
				if(StepsData->CameraShake != nullptr)
				{
					float intensity = !movementComponent->GetSprint() ? 0.2f : 0.4f;
					APlayerController* PlayerController = Cast<APlayerController>(Player->GetInstigatorController());
					PlayerController->ClientStartCameraShake(StepsData->CameraShake, intensity);  
				}

				if(StepsData->SoundCue != nullptr)
				{
					UGameplayStatics::PlaySoundAtLocation(MeshComp, StepsData->SoundCue, HitResult.Location);
				}

				if(StepsData->NiagaraSystem != nullptr)
				{
					UNiagaraFunctionLibrary::SpawnSystemAtLocation(MeshComp, StepsData->NiagaraSystem, HitResult.Location);
				}
				

				//if it is break the function since there s no need for extra checks
				break;
			}
		}

	}
}
