// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifies/VCS_ForceInputFunction_AN.h"


void UVCS_ForceInputFunction_AN::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                        const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if(!FunctionToCall)return;


	FunctionToCall.GetDefaultObject()->FunctionCall(MeshComp);
	//FunctionToCall->FunctionCall(MeshComp);
	
}
