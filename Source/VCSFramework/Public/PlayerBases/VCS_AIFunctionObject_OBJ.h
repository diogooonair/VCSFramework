// Copyright 2023 Diogo Ventura, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "VCS_AIFunctionObject_OBJ.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class VCSFRAMEWORK_API UVCS_AIFunctionObject_OBJ : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, Category = "Triggers")
	void FunctionDefinition(AActor* NPCRef);


	UFUNCTION(BlueprintImplementableEvent, Category = "Triggers")
	bool ShouldPerfom(AActor* NPCRef);
};
