// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifies/VCS_LoopAnimation_ANS.h"

#include "AnimNotifies/VCS_InputFunction_OBJ.h"
#include "Misc/LowLevelTestAdapter.h"


void UVCS_LoopAnimation_ANS::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                         float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	Position = MeshComp->GetPosition();
}

void UVCS_LoopAnimation_ANS::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	
	if(!Condition)return;

	if(!Condition->LoopCondiction(MeshComp))
	{
		MeshComp->SetPosition(Position - 0.1);
	}
	
}

void UVCS_LoopAnimation_ANS::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	if(!bShouldSkipOnceConditionIsMet || !Condition)return;

	if(!Condition->LoopCondiction(MeshComp))return;

	MeshComp->GetAnimInstance()->Montage_SetNextSection(Animation->GetFName(), SectionName);
	
}
