// Copyright 2023 Diogo Ventura, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotifies/VCS_InputFunction_OBJ.h"
#include "VCS_ForceInputFunction_AN.generated.h"

/**
 * 
 */

UCLASS()
class VCSFRAMEWORK_API UVCS_ForceInputFunction_AN : public UAnimNotify
{
	GENERATED_BODY()

protected:
	void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Notify")
	TSubclassOf<UVCS_InputFunction_OBJ> FunctionToCall = nullptr;
};
