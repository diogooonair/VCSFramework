// Copyright 2023 Diogo Ventura, All Rights Reserved.


#include "PlayerBases/VCS_BaseWeapon_Actor.h"
#include "Components/StaticMeshComponent.h"



// Sets default values
AVCS_BaseWeapon_Actor::AVCS_BaseWeapon_Actor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	WeaponSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	WeaponSkeletalMesh->SetupAttachment(RootComponent);
	WeaponStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	WeaponStaticMesh->SetupAttachment(RootComponent);
	
	
}

void AVCS_BaseWeapon_Actor::BeginPlay()
{
	Super::BeginPlay();

	if(WeaponMeshSkeletal)
	{
		WeaponSkeletalMesh->SetSkeletalMesh(WeaponMeshSkeletal);
	}

	if(WeaponMeshSkeletal)
	{
		WeaponStaticMesh->SetStaticMesh(WeaponMeshStatic);
	}
}

UMeshComponent* AVCS_BaseWeapon_Actor::GetWeaponMesh()
{
	if(WeaponMeshSkeletal)return WeaponSkeletalMesh;

	return WeaponStaticMesh;
	
}


