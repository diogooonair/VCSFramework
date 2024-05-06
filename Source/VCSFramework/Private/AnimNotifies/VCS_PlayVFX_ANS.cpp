// Copyright 2023 Diogo Ventura, All Rights Reserved.


#include "AnimNotifies/VCS_PlayVFX_ANS.h"
#include "NiagaraFunctionLibrary.h"
#include "Sound/SoundCue.h"
#include "ActorComponents/VCS_CombatComponent_AC.h"
#include "NiagaraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerBases/VCS_BaseWeapon_Actor.h"

void UVCS_PlayVFX_ANS::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration,
                                   const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	UVCS_CombatComponent_AC* Combat = MeshComp->GetOwner()->FindComponentByClass<UVCS_CombatComponent_AC>();
	AVCS_BaseWeapon_Actor* weapon = nullptr;
	if(Combat)
		weapon = Combat->Weapon;

	if(VFXToPlay){
		if(VFXInstruction == EAttachInstruction::Player)
		{
			NiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(
			VFXToPlay,          
			MeshComp,               
			FName(""),            
			FVector::ZeroVector,             
			FRotator::ZeroRotator,           
			EAttachLocation::KeepRelativeOffset, 
			true                             
			);
		}else
		{
			if (weapon)
			{
				NiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(VFXToPlay,          weapon->GetWeaponMesh(),            FName("WeaponStart"),            FVector::ZeroVector,             FRotator::ZeroRotator,           EAttachLocation::KeepRelativeOffset, true);
			}
		}
	}

	if(SoundToPlay)
	{
		if(weapon){
		
			if(VFXInstruction == EAttachInstruction::Player)
			{
				UGameplayStatics::PlaySoundAtLocation(MeshComp, SoundToPlay, MeshComp->GetOwner()->GetActorLocation(), MeshComp->GetOwner()->GetActorRotation());
			}else
			{
				if (weapon)
				{
					UGameplayStatics::PlaySoundAtLocation(MeshComp, SoundToPlay, weapon->GetActorLocation(), weapon->GetActorRotation());
				}
			}
		}
	}
}



void UVCS_PlayVFX_ANS::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	if (NiagaraComponent)
	{
		//NiagaraComponent->Deactivate();
		//NiagaraComponent->DestroyComponent();
		NiagaraComponent->SetFloatParameter("DissolveAmount", 1.0f);
		NiagaraComponent->Deactivate();
		

		FTimerHandle TimerHandle;
		MeshComp->GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UVCS_PlayVFX_ANS::Timer, 5.0f, false);
		
	}
}

void UVCS_PlayVFX_ANS::Timer()
{
	if(NiagaraComponent->IsValidLowLevel())NiagaraComponent->DestroyComponent();
}
