// Copyright 2023 Diogo Ventura, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/TimelineComponent.h"
#include "VCS_MovementComponent_AC.generated.h"

class UDataTable;
struct FInputActionValue;
class UInputAction;
class UVCS_StateManagerComponent_AC;



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VCSFRAMEWORK_API UVCS_MovementComponent_AC : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UVCS_MovementComponent_AC();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="MovementComponent")
	UInputAction* InputMove;
 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="MovementComponent")
	UInputAction* InputLook;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="MovementComponent")
	UInputAction* InputSprint;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="MovementComponent")
	UDataTable* StepsData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="MovementComponent")
	float sensitivity = 15.f;


	//Vaulting Variable Setup
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="VaultingComponent")
	bool bEnableVaulting;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="VaultingComponent")
	float VaultingMaxLenght = 180.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="VaultingComponent")
	float VaultingMaxHeight = 60.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="VaultingComponent")
	float VaultingMinHeight = 30.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="VaultingComponent")
	UAnimMontage* VaultMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="VaultingComponent")
	UCurveFloat* VaultCurveFloat;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="VaultingComponent")
	float ZInterpMargin = 100.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="VaultingComponent")
	FName VaultState = "";
	

protected:
	void SetupVariables();
	// Called when the game starts
	virtual void BeginPlay() override;

	float sideinput, forwardinput;

	bool IsSprinting;

	UVCS_StateManagerComponent_AC* StateComponent;
	ACharacter* PlayerRef;
	FName LastState;
	FVector2d MouseInput;

	//Vaulting related
	UFUNCTION(BlueprintCallable, Category="VaultingComponent")
	bool CanVaultTrace(float& VaultZ);
	UFUNCTION(BlueprintCallable, Category="VaultingComponent")
	void DoVault(float Z);
	UFUNCTION()
	void AdjustHeightDuringMontage(float Alpha);

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//Get Input Values
	UFUNCTION(BlueprintPure, Category="MovementComponent")
	FVector2D GetInputValues();
	UFUNCTION(BlueprintPure, Category="MovementComponent")
	FVector2D GetMouseInputValue();

	UFUNCTION(BlueprintPure, Category="MovementComponent")
	bool GetSprint();

	UFUNCTION(BlueprintNativeEvent, Category = "MovementComponent")
	void InputOverride(UInputAction* InputAction, ETriggerEvent State, const FInputActionValue& value);
	
#pragma region  MovementInputs
	void MovementInput(const FInputActionValue& Value);
	void LookInput(const FInputActionValue& Value);
	
	void SprintInput(const FInputActionValue& Value);
#pragma region endregion

	//OVERRIDE EVENTS
	UFUNCTION(BlueprintImplementableEvent, Category="MovementComponent")
	void OnNPCHit(AActor* actor);
	
};
