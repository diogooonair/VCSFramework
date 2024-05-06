// Copyright 2023 Diogo Ventura, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "VCS_PlayVFX_ANS.generated.h"

class USoundCue;
class UNiagaraSystem;
class UNiagaraComponent;
/**
 * 
 */
UENUM(BlueprintType)
enum class EAttachInstruction : uint8
{
	Player,
	Sword
};

UCLASS()
class VCSFRAMEWORK_API UVCS_PlayVFX_ANS : public UAnimNotifyState
{
	GENERATED_BODY()

protected:
	UNiagaraComponent* NiagaraComponent = nullptr;
	void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;


	void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	void Timer();

public:
	UPROPERTY(EditAnywhere, Category="Notify")
	UNiagaraSystem* VFXToPlay;

	UPROPERTY(EditAnywhere, Category="Notify")
	EAttachInstruction VFXInstruction;

	UPROPERTY(EditAnywhere, Category="Notify")
	USoundCue* SoundToPlay;

	UPROPERTY(EditAnywhere, Category="Notify")
	EAttachInstruction instruction;
};
