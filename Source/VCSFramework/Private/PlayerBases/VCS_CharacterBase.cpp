// Copyright 2023 Diogo Ventura, All Rights Reserved.


#include "PlayerBases/VCS_CharacterBase.h"

// Sets default values
AVCS_CharacterBase::AVCS_CharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AVCS_CharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AVCS_CharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AVCS_CharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

