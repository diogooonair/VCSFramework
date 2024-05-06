// Copyright 2023 Diogo Ventura, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VCS_LevelManager_A.generated.h"
class AVCS_CharacterBase;
class UVCS_InputFunction_OBJ;
class UUserWidget;

UCLASS()
class VCSFRAMEWORK_API AVCS_LevelManager_A : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AVCS_LevelManager_A();
	UFUNCTION(BlueprintCallable, Category="LevelManager")
	void LoadBackGroundLevel(TSoftObjectPtr<UWorld> LevelToLoad, float loadtime, FName LoadingState, AVCS_CharacterBase* PlayerRef,
							 TSubclassOf<UUserWidget> LoadingScreen, UVCS_InputFunction_OBJ* CallAfterLevelLoading, bool bAssumeDefaultState);

	

private:
	UVCS_InputFunction_OBJ* FunctionCall;
	FName CurrentState;
	UUserWidget* LoadingScreenWidget;
	AVCS_CharacterBase* Player;
	FVector SpawnLoc;
	float LoadTime;
	void HandleMapSwap();
	void AfterLoad();
};
