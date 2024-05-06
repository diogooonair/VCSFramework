// Copyright 2023 Diogo Ventura, All Rights Reserved.


#include "AnimNotifies/VCS_ChangeState_AN.h"

#include "ActorComponents/VCS_StateManagerComponent_AC.h"

void UVCS_ChangeState_AN::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                 const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	UVCS_StateManagerComponent_AC* Comp = MeshComp->GetOwner()->FindComponentByClass<UVCS_StateManagerComponent_AC>();
	if(!Comp)return;
	if(!Comp->IsStateValid(DesiredState))return;
	Comp->CurrentState = DesiredState;
}
