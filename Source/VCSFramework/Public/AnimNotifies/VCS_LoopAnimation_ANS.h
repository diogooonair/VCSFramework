// Copyright 2023 Diogo Ventura, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "VCS_LoopAnimation_ANS.generated.h"

class UVCS_InputFunction_OBJ;
/**
 * 
 */
UCLASS()
class VCSFRAMEWORK_API UVCS_LoopAnimation_ANS : public UAnimNotifyState
{
	GENERATED_BODY()
private:
	float Position = 0.f;
protected:
	void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AnimNotify")
	UVCS_InputFunction_OBJ* Condition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AnimNotify")
	bool bShouldSkipOnceConditionIsMet = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AnimNotify")
	FName SectionName;
};
