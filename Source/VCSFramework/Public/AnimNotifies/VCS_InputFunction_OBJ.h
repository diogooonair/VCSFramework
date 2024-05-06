// Copyright 2023 Diogo Ventura, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "VCS_InputFunction_OBJ.generated.h"

class UVCS_InputCall_ANS;
class AVCS_CharacterBase;
class UNiagaraSystem;
/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class VCSFRAMEWORK_API UVCS_InputFunction_OBJ : public UObject
{
	GENERATED_BODY()

protected:
	UVCS_InputCall_ANS* ref = nullptr;

public:
	void SetInputCallRef(UVCS_InputCall_ANS* notifyref);

	UFUNCTION(BlueprintCallable, Category="Functions")
	void EndInputTracking(USkeletalMeshComponent* Mesh);
	
	
	
	UFUNCTION(BlueprintImplementableEvent,BlueprintCallable ,Category = "Triggers")
	void FunctionTriggered(float fHoldTime, USkeletalMeshComponent* OwnerMesh);

	UFUNCTION(BlueprintImplementableEvent,BlueprintCallable , Category = "Triggers")
	void Hitted(struct FHitResult hitresult);

	UFUNCTION(BlueprintImplementableEvent,BlueprintCallable , Category = "Triggers")
	void OnLevelLoaded(AVCS_CharacterBase* player);

	UFUNCTION(BlueprintImplementableEvent,BlueprintCallable , Category = "Triggers")
	void FunctionCall(USkeletalMeshComponent* Mesh);

	UFUNCTION(BlueprintNativeEvent,BlueprintCallable , Category = "Triggers")
	bool LoopCondiction(USkeletalMeshComponent* Mesh);

	UFUNCTION(BlueprintImplementableEvent,BlueprintCallable , Category = "Triggers")
	void AbilityFunction(AVCS_CharacterBase* Player, AActor* Targetted);
	
	UFUNCTION(BlueprintCallable,BlueprintCallable , Category="HitReact")
	void SpawnBloodVFX(UNiagaraSystem* NiagaraSystem, FVector location, AActor* HitActor);

};
