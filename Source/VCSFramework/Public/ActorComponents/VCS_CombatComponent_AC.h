// Copyright 2023 Diogo Ventura, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "VCS_CombatComponent_AC.generated.h"


class AVCS_BaseWeapon_Actor;
struct FInputActionValue;
class UVCS_StateManagerComponent_AC;
class UAnimMontage;
class ACharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VCSFRAMEWORK_API UVCS_CombatComponent_AC : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UVCS_CombatComponent_AC();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	void SetupVariables();

	UPROPERTY()
	float Health;
	float Stamina;
	float Armor;

	AActor* TargetActor;

	UVCS_StateManagerComponent_AC* PlayerStateComponent;

	FName LastState;

	void TargetActorInput(const FInputActionValue& Value);

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//Obliged Definition Variables
	UPROPERTY(BlueprintReadWrite, Category="CombatComponent")
	AVCS_BaseWeapon_Actor* Weapon;

	
	//Blueprint CallableFunctions
	UFUNCTION(BlueprintPure, Category="CombatComponent")
	float GetHealth();
	UFUNCTION(BlueprintPure, Category="CombatComponent")
	float GetArmor();
	UFUNCTION(BlueprintPure, Category="CombatComponent")
	float GetStamina();
	

	


	UFUNCTION(BlueprintCallable, Category="CombatComponent")
	float TakeHealth(float Damage, bool bCountArmor);
	UFUNCTION(BlueprintCallable, Category="CombatComponent")
	void Target(AActor* actor);
	UFUNCTION(BlueprintCallable, Category="CombatComponent")
	void UnTarget();
	void OnMontageEnd();
	UFUNCTION(BlueprintCallable, Category="CombatComponent")
	void PerformAttack(FName AttackingState, UAnimMontage* Montage, float montagespeed);
	void ResetState();

	UFUNCTION(BlueprintCallable, Category="CombatComponent")
	ACharacter* GetTargetOrPossible();
	UFUNCTION(BlueprintCallable, Category="CombatComponent")
	ACharacter* GetClosestActorToLeftOrRightOfTarget(bool bToLeft);

	UFUNCTION(BlueprintCallable, Category="CombatComponent")
	void PerformFinisher(ACharacter* Killer, ACharacter* Victim, UAnimMontage* KillerMontage, UAnimMontage* VictimMontage, FName KillerState, FName
	                     VictimState, float DesiredDistance);

	UFUNCTION(BlueprintCallable, Category="CombatComponent")
	AActor* GetTargetActor();


	//Configurable Variables
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="CombatComponent")
	class UInputAction* TargetInput;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="CombatComponent")
	float DefaultHealth = 100.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="CombatComponent")
	float DefaultArmor = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="CombatComponent")
	float DefaultStamina = 100.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="CombatComponent")
	float CameraLockRange = 1500.f;

	UPROPERTY(BlueprintReadWrite, Category="CombatComponent")
	bool bShouldTarget = true;

		
};
