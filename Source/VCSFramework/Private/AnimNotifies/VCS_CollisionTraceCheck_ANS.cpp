// Copyright 2023 Diogo Ventura, All Rights Reserved.


#include "AnimNotifies/VCS_CollisionTraceCheck_ANS.h"

#include "ActorComponents/VCS_CombatComponent_AC.h"
#include "AnimNotifies/VCS_InputFunction_OBJ.h"
#include "Kismet/KismetSystemLibrary.h"
#include "VCSFramework/Public/PlayerBases/VCS_BaseWeapon_Actor.h"

void UVCS_CollisionTraceCheck_ANS::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                               float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if(UVCS_CombatComponent_AC* combatcomponent = MeshComp->GetOwner()->FindComponentByClass<UVCS_CombatComponent_AC>())
		Weapon = combatcomponent->Weapon;
	TraceHits.Empty();
}

void UVCS_CollisionTraceCheck_ANS::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
	if(!Weapon || Weapon == nullptr)return;

	UMeshComponent* Mesh = Weapon->GetWeaponMesh();
	if(!Mesh || Mesh == nullptr)return;

	if(!(Mesh->DoesSocketExist("WeaponStart")) || !(Mesh->DoesSocketExist("WeaponEnd")))return;
	
	FVector SocketStart = Mesh->GetSocketLocation("WeaponStart");
	FVector SocketEnd = Mesh->GetSocketLocation("WeaponEnd");


	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));

	// Additional parameters for the trace
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(MeshComp->GetOwner()); // Ignore the actor initiating the trace

	// Array to store hit results
	TArray<FHitResult> OutHits;

	// Perform the multi-sphere trace
	UKismetSystemLibrary::SphereTraceMultiForObjects(
		MeshComp,
		SocketStart,
		SocketEnd,
		20.f,
		ObjectTypes,
		false, // bTraceComplex
		TArray<AActor*>(), // ActorsToIgnore
		EDrawDebugTrace::None,
		OutHits,
		true // bIgnoreSelf
	);

	// Process the hit results
	for (const FHitResult& Hit : OutHits)
	{
		LastHitActor = Hit.GetActor();
		if(!TraceHits.Contains(Hit.GetActor())){TraceHits.Add(Hit.GetActor());sFunctionCall.GetDefaultObject()->Hitted(Hit);}
	}
}


