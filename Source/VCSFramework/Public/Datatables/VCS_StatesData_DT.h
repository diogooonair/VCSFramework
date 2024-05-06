// Copyright 2023 Diogo Ventura, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "VCS_StatesData_DT.generated.h"

USTRUCT(BlueprintType, Category="StateComponent")
struct FStateDataStruct: public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="StateComponent")
	FName StateName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="StateComponent")
	FString AllowedInputs;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="StateComponent")
	UBlendSpace* BlendSpace;
};


UCLASS()
class VCSFRAMEWORK_API AVCS_StatesData_DT : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AVCS_StatesData_DT();
	
	UPROPERTY(BlueprintReadWrite, Category="StateComponent")
	UDataTable* DStatesTable;
};
