// Copyright 2023 Diogo Ventura, All Rights Reserved.


#include "ActorComponents/VCS_AILibrary_AC.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "ActorComponents/VCS_StateManagerComponent_AC.h"
#include "GameFramework/Character.h"
#include "PlayerBases/VCS_AIFunctionObject_OBJ.h"

// Sets default values for this component's properties
UVCS_AILibrary_AC::UVCS_AILibrary_AC(): NPCState(nullptr), bAllowBehave(true), TimerInterval(.2)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UVCS_AILibrary_AC::BeginPlay()
{
	Super::BeginPlay();

	NPCState = Cast<UVCS_StateManagerComponent_AC>(GetOwner()->GetComponentByClass(UVCS_StateManagerComponent_AC::StaticClass()));
	
	FTimerHandle NPCBaseThinking;
	
	GetWorld()->GetTimerManager().SetTimer(NPCBaseThinking, this, &UVCS_AILibrary_AC::TimerCallback, TimerInterval, true);
	
}



// Define the timer callback function
void UVCS_AILibrary_AC::TimerCallback()
{
	if(!bAllowBehave)return;
	
	if(!NPCState)return;
	
	//Get Thinking function based on your ifs
	UVCS_AIFunctionObject_OBJ* OBJ = NPCState->GetExecutableFunction();
	if(!OBJ){return;}
	OBJ->FunctionDefinition(GetOwner());
}


// Called every frame
void UVCS_AILibrary_AC::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UVCS_AILibrary_AC::SetAllowBehave(bool value)
{
	bAllowBehave = value;
}

void UVCS_AILibrary_AC::PerformFunction(FName FunctionName)
{
	if(!NPCState)return;

	//Stop Behaving before function execution
	bAllowBehave = false;
	UVCS_AIFunctionObject_OBJ* Function = NPCState->GetSpecificFunction(FunctionName);
	Function->FunctionDefinition(Cast<ACharacter>(GetOwner()));

	bAllowBehave = true;
}

EHitDirection UVCS_AILibrary_AC::GetDirection(FVector HitNormal, FVector RightVector, FVector ForwardVector)
{
	float DotForward = FVector::DotProduct(HitNormal, ForwardVector);
	
	float DotRight = FVector::DotProduct(HitNormal, RightVector);

	// Use the dot products to determine the hit direction
	if (FMath::Abs(DotForward) > FMath::Abs(DotRight))
	{
		// Hit is more in the forward or backward direction
		if (DotForward > 0.0f)
		{
			return EHitDirection::Forward;
		}
		else
		{
			return EHitDirection::Backward;
		}
	}
	else
	{
		// Hit is more in the left or right direction
		if (DotRight > 0.0f)
		{
			return EHitDirection::Right;
		}
		else
		{
			return EHitDirection::Left;
		}
	}

	// Unknown direction (this should not happen, but you can handle it if needed)
	return EHitDirection::Unknown;
}

void UVCS_AILibrary_AC::SetAllowCustomAction(bool value)
{
	bAllowCustomAction = value;
}

bool UVCS_AILibrary_AC::GetAllowCustomAction()
{
	return bAllowCustomAction;
}

