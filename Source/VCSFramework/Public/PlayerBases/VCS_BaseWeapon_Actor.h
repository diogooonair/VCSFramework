// Copyright 2023 Diogo Ventura, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VCS_BaseWeapon_Actor.generated.h"

UCLASS()
class VCSFRAMEWORK_API AVCS_BaseWeapon_Actor : public AActor
{
	GENERATED_BODY()

protected:
	class USkeletalMeshComponent* WeaponSkeletalMesh;
	class UStaticMeshComponent* WeaponStaticMesh;

	virtual void BeginPlay() override;
	
public:	
	// Sets default values for this actor's properties
	AVCS_BaseWeapon_Actor();

	UFUNCTION(BlueprintCallable)
	void SpawnFixer();
	

	
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	class UStaticMesh* WeaponMeshStatic;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	class USkeletalMesh* WeaponMeshSkeletal;
	
	UFUNCTION(BlueprintCallable, Category="WeaponComponent")
	class UMeshComponent* GetWeaponMesh();
};
