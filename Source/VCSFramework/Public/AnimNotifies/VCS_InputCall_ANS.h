// Copyright 2023 Diogo Ventura, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "VCS_InputCall_ANS.generated.h"

struct FInputActionInstance;
class UInputAction;
class UEnhancedInputLocalPlayerSubsystem;
class UEnhancedPlayerInput;
class UVCS_InputFunction_OBJ;

USTRUCT(BlueprintType)
struct FAnimSetup
{
	GENERATED_USTRUCT_BODY()
	


public:

	int32 Started;
	int32 Triggered;
	//Input for animnotifytrack
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Notify")
	UInputAction* EInput;

	//Function to call on Input
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Notify")
	TSubclassOf<UVCS_InputFunction_OBJ> sFunctionCall;
	
	//Majorlly for combo actions if the input inst done by the end of the notify tick, the anim montage stops
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Notify")
	bool bStopOnMiss;

	//Stamina Cost of action
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Notify")
	float Stamina;

	//Enable Hold Input
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Notify")
	bool bHold;

	FAnimSetup()
	{
		// Set default values for the properties
		EInput = nullptr;
		sFunctionCall = nullptr;
		bStopOnMiss = false;
		bHold = false;
		Started = 0;
		Triggered = 0;
	}
};



/**
 * 
 */
UCLASS()
class VCSFRAMEWORK_API UVCS_InputCall_ANS : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	//Setup the Array property for actions
	UPROPERTY(EditAnywhere, Category = "ActionSetup")
	TArray<FAnimSetup> ActionList;

	//UPROPERTY(EditAnywhere, Category="ActionSetup")
	public:
	UVCS_InputCall_ANS()
	{
		// Create a copy of the ActionList property
		ActionListCopy = ActionList;
	}

	void UnBindInput(USkeletalMeshComponent* MeshComp);

private:
	// Copy of the ActionList property
	TArray<FAnimSetup> ActionListCopy;

protected:
	
	void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	void InputTrigger(const FInputActionInstance& ActionInstance);
	void InputStarted(const FInputActionInstance& ActionInstance);
	
	
	USkeletalMeshComponent* OwnerMesh = nullptr;
};
