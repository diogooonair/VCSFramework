// Copyright 2023 Diogo Ventura, All Rights Reserved.


#include "PlayerBases/VCS_BaseWeapon_Actor.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Character.h"


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

void AVCS_BaseWeapon_Actor::SpawnFixer()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("ue2"));

	if(WeaponMeshSkeletal)
	{
		WeaponSkeletalMesh->SetSkeletalMesh(WeaponMeshSkeletal);
		
	}
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("ue1"));
	if(WeaponMeshSkeletal)
	{
		WeaponStaticMesh->SetStaticMesh(WeaponMeshStatic);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT("ue"));
	}
}

void AVCS_BaseWeapon_Actor::BeginPlay()
{
	Super::BeginPlay();
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("ue2"));
	SetActorEnableCollision(false);

	if(WeaponMeshSkeletal)
	{
		WeaponSkeletalMesh->SetSkeletalMesh(WeaponMeshSkeletal);
		
	}
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("ue1"));
	if(WeaponMeshStatic)
	{
		WeaponStaticMesh->SetStaticMesh(WeaponMeshStatic);
		//WeaponStaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		//WeaponStaticMesh->AttachToComponent(Cast<ACharacter>(GetOwner())->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, "Katana");
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT("ue3"));
	}
}

UMeshComponent* AVCS_BaseWeapon_Actor::GetWeaponMesh()
{
	if(WeaponMeshSkeletal)return WeaponSkeletalMesh;

	return WeaponStaticMesh;
	
}


