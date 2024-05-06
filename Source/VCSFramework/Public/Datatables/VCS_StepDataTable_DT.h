// Copyright 2023 Diogo Ventura, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GameFramework/Actor.h"
#include "NiagaraSystem.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Camera/CameraShakeBase.h"
#include "VCS_StepDataTable_DT.generated.h"

class USoundCue;


USTRUCT(BlueprintType, Category="StepNotify")
struct FStepDataStruct: public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="StepNotify")
	UPhysicalMaterial* PhysicalMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="StepNotify")
	TSubclassOf<UCameraShakeBase> CameraShake;
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="StepNotify")
	USoundCue* SoundCue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="StepNotify")
	UNiagaraSystem* NiagaraSystem;
};

UCLASS()
class VCSFRAMEWORK_API AVCS_StepDataTable_DT : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AVCS_StepDataTable_DT();

	UPROPERTY(BlueprintReadWrite, Category="StateComponent")
	UDataTable* DStatesTable;

};
