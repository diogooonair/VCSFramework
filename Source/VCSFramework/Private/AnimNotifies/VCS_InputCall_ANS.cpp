// Copyright 2023 Diogo Ventura, All Rights Reserved.


#include "AnimNotifies/VCS_InputCall_ANS.h"

#include "InputTriggers.h"
#include "EnhancedInputComponent.h"
#include "ActorComponents/VCS_CombatComponent_AC.h"
#include "AnimNotifies/VCS_InputFunction_OBJ.h"
#include "Engine/LocalPlayer.h"
#include "Animation/AnimInstance.h"

bool triggered = false;
void UVCS_InputCall_ANS::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                     float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	ActionListCopy = ActionList;
	OwnerMesh = MeshComp;
	

	APlayerController* PC = Cast<APlayerController>(MeshComp->GetOwner()->GetInstigatorController());
	if(PC){
		UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PC->GetComponentByClass(UEnhancedInputComponent::StaticClass()));

		if(!Input)return;
		//Loop through all setup actions and start tracking the input
		for(int i = 0; i < ActionListCopy.Num(); i++)
		{
			ActionList[i].Triggered = Input->BindAction(ActionListCopy[i].EInput, ETriggerEvent::Triggered, this, &UVCS_InputCall_ANS::InputTrigger).GetHandle();
			ActionList[i].Started = Input->BindAction(ActionListCopy[i].EInput, ETriggerEvent::Started, this, &UVCS_InputCall_ANS::InputStarted).GetHandle();
		}}

	for(int i = 0; i < ActionListCopy.Num(); i++)
	{
		if(ActionListCopy[i].sFunctionCall)
			ActionListCopy[i].sFunctionCall.GetDefaultObject()->SetInputCallRef(this);
	}
}

void UVCS_InputCall_ANS::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	APlayerController* PC = Cast<APlayerController>(MeshComp->GetOwner()->GetInstigatorController());
	//Stop Playing montage on input miss, Combo only

	if(!PC)return;
	
	UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PC->GetComponentByClass(UEnhancedInputComponent::StaticClass()));
	if(!Input)return;
	
	//if the stop on miss is true in any of the events stop playing montage
	for(int i = 0; i < ActionListCopy.Num(); i++)
	{
		if(ActionListCopy[i].bStopOnMiss)
		{
			MeshComp->GetAnimInstance()->StopAllMontages(0.2);
			MeshComp->GetOwner()->FindComponentByClass<UVCS_CombatComponent_AC>()->ResetState();
		}
	}

	UnBindInput(MeshComp);
}

void UVCS_InputCall_ANS::InputTrigger(const FInputActionInstance& ActionInstance)
{
	//Find The correct inout and exec Action
	const UInputAction* ActionTriggered = ActionInstance.GetSourceAction();
	for(int i = 0; i < ActionListCopy.Num(); i++)
	{
		if(ActionTriggered == ActionListCopy[i].EInput && ActionList[i].bHold)
		{
			if(ActionListCopy[i].sFunctionCall)
				ActionListCopy[i].sFunctionCall.GetDefaultObject()->FunctionTriggered(ActionInstance.GetTriggeredTime(), OwnerMesh);
			ActionListCopy.RemoveAt(i);
		}
	}
}

void UVCS_InputCall_ANS::InputStarted(const FInputActionInstance& ActionInstance)
{
	//On input start , check if the input has bHold, else performAction
	const UInputAction* ActionTriggered = ActionInstance.GetSourceAction();
	for(int i = 0; i < ActionListCopy.Num(); i++)
	{
		if(ActionTriggered == ActionListCopy[i].EInput)
		{
			if(ActionListCopy[i].bHold)
				continue;

			if(ActionListCopy[i].sFunctionCall)
				ActionListCopy[i].sFunctionCall.GetDefaultObject()->FunctionTriggered(0, OwnerMesh);
			
			ActionListCopy.RemoveAt(i);
		}
	}
}

void UVCS_InputCall_ANS::UnBindInput(USkeletalMeshComponent* MeshComp)
{
	APlayerController* PC = Cast<APlayerController>(MeshComp->GetOwner()->GetInstigatorController());
	if(PC){
		UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PC->GetComponentByClass(UEnhancedInputComponent::StaticClass()));

		if(!Input)return;
		//Loop through all setup actions and start tracking the input
		for(int i = 0; i < ActionList.Num(); i++)
		{
			Input->RemoveBindingByHandle(ActionList[i].Triggered);
			Input->RemoveBindingByHandle(ActionList[i].Started);
		}}
}
