// Copyright 2023 Diogo Ventura, All Rights Reserved.


#include "ActorComponents/VCS_StateManagerComponent_AC.h"

#include "InputAction.h"
#include "Datatables/VCS_AIDataTables_DT.h"
#include "PlayerBases/VCS_AIFunctionObject_OBJ.h"


// Sets default values for this component's properties
UVCS_StateManagerComponent_AC::UVCS_StateManagerComponent_AC(): currentblendspace(nullptr), DStatesTable(nullptr),
                                                                IsAI(false),
                                                                AITable(nullptr)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;


	// ...
}


// Called when the game starts
void UVCS_StateManagerComponent_AC::BeginPlay()
{
	Super::BeginPlay();
	CurrentState = DefaultState;
}


UVCS_AIFunctionObject_OBJ* UVCS_StateManagerComponent_AC::GetExecutableFunction()
{
	if(!IsAI)return nullptr;
	TArray<FActionsDataStruct*> ActionDataArray;
	AITable->GetAllRows<FActionsDataStruct>("", ActionDataArray);

	for (const FActionsDataStruct* ActionsData : ActionDataArray)
	{
		if(ActionsData->StateName.IsEqual(CurrentState))
		{
			if(ActionsData->ShouldPerfomObject.GetDefaultObject()->ShouldPerfom(GetOwner()))
			{
				return ActionsData->ActionObject.GetDefaultObject();
			}
		}
	}

	//In Case of no shouldperform, it should return the defaultaction
	for (const FActionsDataStruct* ActionsData : ActionDataArray)
	{
		if(ActionsData->StateName.IsEqual(CurrentState) && ActionsData->bIsDefault)
		{
			return ActionsData->ActionObject.GetDefaultObject();
		}
	}

	return nullptr;
}

UVCS_AIFunctionObject_OBJ* UVCS_StateManagerComponent_AC::GetSpecificFunction(FName name)
{
	if(!IsAI)return nullptr;
	TArray<FActionsDataStruct*> ActionDataArray;
	AITable->GetAllRows<FActionsDataStruct>("", ActionDataArray);

	for (const FActionsDataStruct* ActionsData : ActionDataArray)
	{
		if(ActionsData->StateName.IsEqual(CurrentState) && ActionsData->ActionName.IsEqual(name))
		{
				return ActionsData->ActionObject.GetDefaultObject();
		}
	}
	return nullptr;
}

// Called every frame
void UVCS_StateManagerComponent_AC::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

TArray<FName> UVCS_StateManagerComponent_AC::GetStateNamesFromDataTable()
{
	TArray<FName> StateNames;

	if (DStatesTable)
	{
		TArray<FStateDataStruct*> StateDataArray;
		DStatesTable->GetAllRows<FStateDataStruct>("", StateDataArray);
		

		for (const FStateDataStruct* StateData : StateDataArray)
		{
			StateNames.Add(StateData->StateName);
		}
	}

	return StateNames;
}

//Checks if the action is authorized
bool UVCS_StateManagerComponent_AC::CanPerformAction(UInputAction* Input, FString ActionName)
{
	FString ActionInput;
	TArray<FStateDataStruct*> StateDataArray;
	DStatesTable->GetAllRows<FStateDataStruct>("", StateDataArray);
	
	if(Input != nullptr){
		ActionInput = Input->GetName();
		ActionInput = ActionInput.Append(";");
		if(CustomAllowedInputs.Contains(ActionInput) || (ActionName.Len() > 0 && CustomAllowedInputs.Contains(ActionName.Append(";"))))
			return true;

		ActionInput.InsertAt(0, ";");
		
	}

	for (const FStateDataStruct* StateData : StateDataArray)
	{
		if(StateData->StateName.IsEqual(CurrentState))
		{

			if(Input == nullptr && StateData->AllowedInputs.Contains(ActionName))
			{
				return true;
			}
			
			if(Input != nullptr && StateData->AllowedInputs.Contains(ActionInput))
			{
				return true;
			}

			//if it is break the function since there s no need for extra checks
			break;
		}
	}
	

	return false;
}

UBlendSpace* UVCS_StateManagerComponent_AC::GetCurrentBlendSpace()
{
	TArray<FStateDataStruct*> StateDataArray;
	DStatesTable->GetAllRows<FStateDataStruct>("", StateDataArray);

	for (const FStateDataStruct* StateData : StateDataArray)
	{
		if(StateData->StateName.IsEqual(CurrentState))
		{
			if(!(StateData->BlendSpace)) return currentblendspace;

			currentblendspace = StateData->BlendSpace;
			return StateData->BlendSpace;
		}
	}

	return nullptr;
}

void UVCS_StateManagerComponent_AC::AddActionInput(UInputAction* ActionToAdd)
{
	CustomAllowedInputs.Add(ActionToAdd->GetName().Append(";"));
}

void UVCS_StateManagerComponent_AC::ClearAddedInput(UInputAction* ActionToRemove)
{
	CustomAllowedInputs.Remove(ActionToRemove->GetName().Append(";"));
}

bool UVCS_StateManagerComponent_AC::IsStateValid(FName StateName)
{
	TArray<FStateDataStruct*> StateDataArray;
	DStatesTable->GetAllRows<FStateDataStruct>("", StateDataArray);

	for (const FStateDataStruct* StateData : StateDataArray)
	{
		if(StateData->StateName.IsEqual(StateName))
		{
			return true;
		}
	}

	return false;
}



