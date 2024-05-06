// Copyright 2023 Diogo Ventura, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "VCS_RotateToAI_ANS.generated.h"

/**
 * 
 */
UCLASS()
class VCSFRAMEWORK_API UVCS_RotateToAI_ANS : public UAnimNotifyState
{
	GENERATED_BODY()

protected:
	AActor* ActorToRotate;
	void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;
};
