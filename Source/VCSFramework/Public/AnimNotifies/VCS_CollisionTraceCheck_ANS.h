// Copyright 2023 Diogo Ventura, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "Engine/EngineTypes.h"
#include "VCS_CollisionTraceCheck_ANS.generated.h"

/**
 * 
 */
class UVCS_InputFunction_OBJ;
UCLASS()
class VCSFRAMEWORK_API UVCS_CollisionTraceCheck_ANS : public UAnimNotifyState
{
	GENERATED_BODY()

private:
	TArray<AActor*> TraceHits;
	class AVCS_BaseWeapon_Actor* Weapon;
	AActor* LastHitActor;

protected:
	void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;

public:
	UPROPERTY(EditAnywhere, Category="ActionSetup")
	TSubclassOf<UVCS_InputFunction_OBJ> sFunctionCall;
};
