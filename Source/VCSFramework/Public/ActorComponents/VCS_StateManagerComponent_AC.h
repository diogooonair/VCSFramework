// Copyright 2023 Diogo Ventura, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Datatables/VCS_StatesData_DT.h"
#include "VCS_StateManagerComponent_AC.generated.h"

class UVCS_AIFunctionObject_OBJ;
class UInputAction;
class UDataTable;
class UBlendSpace;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VCSFRAMEWORK_API UVCS_StateManagerComponent_AC : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UVCS_StateManagerComponent_AC();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	TArray<FString> CustomAllowedInputs;

	UBlendSpace* currentblendspace;

	
public:
	//AIFunctions
	UVCS_AIFunctionObject_OBJ* GetExecutableFunction();
	UVCS_AIFunctionObject_OBJ* GetSpecificFunction(FName name);
	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="StateManager")
	UDataTable* DStatesTable;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="StateManager")
	bool IsAI;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="StateManager")
	UDataTable* AITable;
	
	//Function to get all state Names
	UFUNCTION(BlueprintCallable, Category="StateManager")
	TArray<FName> GetStateNamesFromDataTable();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="StateManager")
	FName DefaultState;
	UPROPERTY(BlueprintReadWrite, Category="StateManager")
	FName CurrentState;

	UFUNCTION(BlueprintCallable, Category="StateManager")
	bool CanPerformAction(UInputAction* Input, FString Actionname);

	UFUNCTION(BlueprintPure, Category="StateManager")
	UBlendSpace* GetCurrentBlendSpace();

	void AddActionInput(UInputAction* ActionToAdd);
	void ClearAddedInput(UInputAction* ActionToRemove);

	//Blueprint Callables
	UFUNCTION(BlueprintCallable, Category="StateManager")
	bool IsStateValid(FName StateName);
};
