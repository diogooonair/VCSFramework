// Copyright 2023 Diogo Ventura, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GameFramework/Actor.h"
#include "VCS_AIDataTables_DT.generated.h"

class UVCS_AIFunctionObject_OBJ;

USTRUCT(BlueprintType, Category="StateComponent")
struct FActionsDataStruct: public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="StateComponent")
	FName StateName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="StateComponent")
	FName ActionName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="StateComponent")
	TSubclassOf<UVCS_AIFunctionObject_OBJ> ActionObject;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="StateComponent")
	TSubclassOf<UVCS_AIFunctionObject_OBJ> ShouldPerfomObject;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="StateComponent")
	bool bIsDefault = false;
};

UCLASS()
class VCSFRAMEWORK_API AVCS_AIDataTables_DT : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AVCS_AIDataTables_DT();
	
	UPROPERTY(BlueprintReadWrite, Category="StateComponent")
	UDataTable* DAITable;
	
};
