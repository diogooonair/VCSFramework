// Copyright 2023 Diogo Ventura, All Rights Reserved.


#include "AnimNotifies/VCS_InputFunction_OBJ.h"

#include "NiagaraFunctionLibrary.h"
#include "AnimNotifies/VCS_InputCall_ANS.h"

bool UVCS_InputFunction_OBJ::LoopCondiction_Implementation(USkeletalMeshComponent* Mesh)
{
	return true;
}


void UVCS_InputFunction_OBJ::SetInputCallRef(UVCS_InputCall_ANS* notifyref)
{
	ref = notifyref;
}

void UVCS_InputFunction_OBJ::EndInputTracking(USkeletalMeshComponent* Mesh)
{
	if(!ref)return;
	ref->UnBindInput(Mesh);
	
}


void UVCS_InputFunction_OBJ::SpawnBloodVFX(UNiagaraSystem* NiagaraSystem, FVector location, AActor* HitActor)
{
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(HitActor, NiagaraSystem, location);
}
