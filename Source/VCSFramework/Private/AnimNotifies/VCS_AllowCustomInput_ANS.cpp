// Copyright 2023 Diogo Ventura, All Rights Reserved.


#include "AnimNotifies/VCS_AllowCustomInput_ANS.h"

#include "InputAction.h"
#include "ActorComponents/VCS_StateManagerComponent_AC.h"
#include "Components/SkeletalMeshComponent.h"

void UVCS_AllowCustomInput_ANS::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                            float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if(AActor* Actor = MeshComp->GetOwner())
	{
		UVCS_StateManagerComponent_AC* component = Actor->FindComponentByClass<UVCS_StateManagerComponent_AC>();
		if(!component)return;
	
		for(int i = 0; i < ActionList.Num(); i++)
		{
			component->AddActionInput(ActionList[i]);
		}
	}
	
}

void UVCS_AllowCustomInput_ANS::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if(AActor* Actor = MeshComp->GetOwner())
	{
		UVCS_StateManagerComponent_AC* component = Actor->FindComponentByClass<UVCS_StateManagerComponent_AC>();
		if(!component)return;
		for(int i = 0; i < ActionList.Num(); i++)
		{
			component->ClearAddedInput(ActionList[i]);
		}
	}
	

	
}
