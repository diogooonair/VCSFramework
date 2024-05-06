// Copyright 2023 Diogo Ventura, All Rights Reserved.


#include "LevelManagement/VCS_LevelManager_A.h"

#include "ActorComponents/VCS_StateManagerComponent_AC.h"
#include "AnimNotifies/VCS_InputFunction_OBJ.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerBases/VCS_CharacterBase.h"

// Sets default values
AVCS_LevelManager_A::AVCS_LevelManager_A()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AVCS_LevelManager_A::LoadBackGroundLevel(TSoftObjectPtr<UWorld> LevelToLoad, float loadtime, FName LoadingState, AVCS_CharacterBase* PlayerRef,
							 TSubclassOf<UUserWidget> LoadingScreen, UVCS_InputFunction_OBJ* CallAfterLevelLoading, bool bAssumeDefaultState)
{
	if(!LevelToLoad || !PlayerRef) return;

	Player = PlayerRef;
	LoadTime = loadtime;

	//Define loadinfo for callbacks
	FLatentActionInfo info;
	info.CallbackTarget = this;
	info.ExecutionFunction = "AfterLoad";
	info.UUID = 1;
	info.Linkage = 0;
	
	FunctionCall = CallAfterLevelLoading;
	UVCS_StateManagerComponent_AC* Component = PlayerRef->FindComponentByClass<UVCS_StateManagerComponent_AC>();
	//Make CurrentState blank so if there are no changes the function won t need to do anything
	CurrentState = "";
	if(Component)
	{
		if(Component->IsStateValid(LoadingState))
		{
			if(bAssumeDefaultState) CurrentState = Component->DefaultState;
			else CurrentState = Component->CurrentState;
			
			Component->CurrentState = LoadingState;
		}
	}

	if(LoadingScreen)
	{
		LoadingScreenWidget = CreateWidget(Cast<APlayerController>(PlayerRef->GetController()), LoadingScreen);
		LoadingScreenWidget->AddToViewport();
	}
	
	UGameplayStatics::LoadStreamLevelBySoftObjectPtr(GetWorld(), LevelToLoad, true, false, info);
	SpawnLoc = UGameplayStatics::GetActorOfClass(LevelToLoad->GetWorld(),APlayerStart::StaticClass())->GetActorLocation();
	
	
	//Unload last level
	FLatentActionInfo infop;
	FString CurrentLevel = UGameplayStatics::GetCurrentLevelName(GetWorld());
	
	UGameplayStatics::UnloadStreamLevel(GetWorld(),*CurrentLevel, infop, false);
}

void AVCS_LevelManager_A::HandleMapSwap()
{
	LoadingScreenWidget->RemoveFromParent();
	LoadingScreenWidget->Destruct();
	UVCS_StateManagerComponent_AC* Component = Player->FindComponentByClass<UVCS_StateManagerComponent_AC>();
	if(Component && CurrentState != "")Component->CurrentState = CurrentState;

	if(FunctionCall)FunctionCall->OnLevelLoaded(Player);
}

void AVCS_LevelManager_A::AfterLoad()
{
	if(!Player)return;
	Player->SetActorLocation(SpawnLoc);

	FTimerHandle HandleMapSwap;
	GetWorldTimerManager().SetTimer(HandleMapSwap, this, &AVCS_LevelManager_A::HandleMapSwap, LoadTime, false);
	
	
}


