// Copyright 2023 Diogo Ventura, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "VCS_Camera_AC.generated.h"

class USpringArmComponent;
/**
 * 
 */
UCLASS(Blueprintable)
class VCSFRAMEWORK_API UVCS_Camera_AC : public UCameraComponent
{
	GENERATED_BODY()

public:
	UVCS_Camera_AC();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction);
		
	UPROPERTY(BlueprintReadWrite, Category="CameraComponent")
	float SDeltaTime = 0.0f;

	
	UFUNCTION(BlueprintCallable, Category="CameraComponent")
	void PhysicsCalculus();
	UFUNCTION(BlueprintCallable, Category="CameraComponent")
	void Fov();
	
	APawn* Pawn;
	USpringArmComponent* SpringArm;

	//Variables
	UPROPERTY(BlueprintReadWrite, Category="CameraComponent")
	FVector Current_SocketOffSet;
	UPROPERTY(BlueprintReadOnly, Category="CameraComponent")
	float Current_FieldOfView;

	//Configurable Variables
	UPROPERTY(BlueprintReadWrite, Category="CameraComponent")
	float VelocityRange = 400.0f;
	UPROPERTY(BlueprintReadWrite, Category="CameraComponent")
	float TorqueAmount = 7.0f;

	UPROPERTY(BlueprintReadWrite, Category="CameraComponent")
	float MaxPitch = 25.0f;
	UPROPERTY(BlueprintReadWrite, Category="CameraComponent")
	float MaxRoll = 25.0f;
	UPROPERTY(BlueprintReadWrite, Category="CameraComponent")
	float MaxYaw = 25.0f;
	UPROPERTY(BlueprintReadWrite, Category="CameraComponent")
	float InterpSpeed = 5.0f;
	UPROPERTY(BlueprintReadWrite, Category="CameraComponent")
	float PanAmount = 50.0f;
	UPROPERTY(BlueprintReadWrite, Category="CameraComponent")
	float MaxPanX = 100.0f;
	UPROPERTY(BlueprintReadWrite, Category="CameraComponent")
	float MaxPanY = 100.0f;
	UPROPERTY(BlueprintReadWrite, Category="CameraComponent")
	float MaxPanZ = 100.0f;
	UPROPERTY(BlueprintReadWrite, Category="CameraComponent")
	float FieldOfViewAmount = 4.0f;
	UPROPERTY(BlueprintReadWrite, Category="CameraComponent")
	float MaxFieldOfView = 120.0f;
};
