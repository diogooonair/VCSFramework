// Copyright 2023 Diogo Ventura, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "VCS_AILibrary_AC.generated.h"


class UVCS_StateManagerComponent_AC;

UENUM(BlueprintType)
enum class EHitDirection : uint8
{
	Forward,
	Backward,
	Left,
	Right,
	Unknown
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VCSFRAMEWORK_API UVCS_AILibrary_AC : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UVCS_AILibrary_AC();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	void TimerCallback();
	UVCS_StateManagerComponent_AC* NPCState;
	bool bAllowBehave;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	bool bAllowCustomAction = false;


public:
	UFUNCTION(BlueprintCallable, Category="AILibrary")
	void SetAllowBehave(bool value);
	UFUNCTION(BlueprintCallable, Category="AILibrary")
	void PerformFunction(FName FunctionName);
	UFUNCTION(BlueprintPure, Category="AILibrary")
	static EHitDirection GetDirection(FVector HitNormal, FVector RightVector, FVector ForwardVector);
	UFUNCTION(BlueprintCallable, Category="AILibrary")
	void SetAllowCustomAction(bool value);
	UFUNCTION(BlueprintPure, Category="AILibrary")
	bool GetAllowCustomAction();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="AILibrary")
	float TimerInterval;
};
