// Copyright 2023 Diogo Ventura, All Rights Reserved.


#include "ActorComponents/VCS_MovementComponent_AC.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "ActorComponents/VCS_AILibrary_AC.h"
#include "ActorComponents/VCS_StateManagerComponent_AC.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "PlayerBases/VCS_AIFunctionObject_OBJ.h"

float ZForInterpolation = 0.0f;


// Sets default values for this component's properties
UVCS_MovementComponent_AC::UVCS_MovementComponent_AC() : InputMove(nullptr), InputLook(nullptr), InputSprint(nullptr),
                                                         StepsData(nullptr),
                                                         bEnableVaulting(false), VaultMontage(nullptr),
                                                         VaultCurveFloat(nullptr), sideinput(0),
                                                         forwardinput(0),
                                                         IsSprinting(false),
                                                         StateComponent(nullptr),
                                                         PlayerRef(nullptr)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

void UVCS_MovementComponent_AC::InputOverride_Implementation(UInputAction* InputAction, ETriggerEvent State,
                                                             const FInputActionValue& value)
{
	//Empty function because it needs a definition
}


void UVCS_MovementComponent_AC::SetupVariables()
{
	APlayerController* PC = Cast<APlayerController>(GetOwner()->GetInstigatorController());
	if(PC){
		UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PC->GetComponentByClass(UEnhancedInputComponent::StaticClass()));

		if(Input){
			//Loop through all setup actions and start tracking the input
			//Adding extra input check because unreal did one of those things where it crashes because it feels like it
			if(InputMove && Input)		{
				Input->BindAction(InputMove, ETriggerEvent::Triggered, this, &UVCS_MovementComponent_AC::MovementInput);
				Input->BindAction(InputMove, ETriggerEvent::Completed, this, &UVCS_MovementComponent_AC::MovementInput);
			}

			if(InputLook && Input)
			{
				Input->BindAction(InputLook, ETriggerEvent::Triggered, this, &UVCS_MovementComponent_AC::LookInput);
			}
	

			if(InputSprint && Input)
			{
				Input->BindAction(InputSprint, ETriggerEvent::Started, this, &UVCS_MovementComponent_AC::SprintInput);
				Input->BindAction(InputSprint, ETriggerEvent::Completed, this, &UVCS_MovementComponent_AC::SprintInput);
			}}
	}
	
	PlayerRef = Cast<ACharacter>(GetOwner());
	if(PlayerRef)
	{
		StateComponent = Cast<UVCS_StateManagerComponent_AC>(PlayerRef->GetComponentByClass(UVCS_StateManagerComponent_AC::StaticClass()));
	}
	
	
}

// Called when the game starts
void UVCS_MovementComponent_AC::BeginPlay()
{
	Super::BeginPlay();

	// ...

	FTimerHandle DelayTimer;
	GetWorld()->GetTimerManager().SetTimer(
		DelayTimer, this, &UVCS_MovementComponent_AC::SetupVariables, 0.1f, false);

	
}
//////////////////////////
///Vaulting/////////
/////////////////////
bool UVCS_MovementComponent_AC::CanVaultTrace(float& VaultZ)
{
    
	if(PlayerRef->GetMovementComponent()->IsFalling())return false;
	const FVector ActorLocation = GetOwner()->GetActorLocation();
	const FVector ActorForwardVector = GetOwner()->GetActorForwardVector();

	// Set the trace start and end points
	const FVector TraceStart = ActorLocation + ActorForwardVector * 80.f + FVector(0.f, 0.f, VaultingMinHeight);
	const FVector TraceEnd = TraceStart + ActorForwardVector * 80.f;

	// Perform multi-trace
	TArray<FHitResult> HitResults;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(GetOwner());

	bool bHit = GetWorld()->SweepMultiByChannel(HitResults, TraceStart, TraceEnd, FQuat::Identity, ECC_Visibility, FCollisionShape::MakeBox(FVector(50.f, 50.f, (VaultingMaxHeight - VaultingMinHeight) / 2.0f)), CollisionParams);

	if (bHit)
	{
		// Find the highest point
		FHitResult HighestHit = HitResults[0];
		for (const FHitResult& HitResult : HitResults)
		{
			if(HitResult.GetActor()->FindComponentByClass<UVCS_AILibrary_AC>()){OnNPCHit(HitResult.GetActor()); return false;}
			
			if (HitResult.ImpactPoint.Z > HighestHit.ImpactPoint.Z)
			{
				HighestHit = HitResult;
			}
		}

		// Calculate the height of the vaultable object
		VaultZ  = HighestHit.ImpactPoint.Z;

		USkeletalMeshComponent* mesh = GetOwner()->FindComponentByClass<USkeletalMeshComponent>();
		if(!mesh)return false;
		
		const FVector EyesLocation = mesh->GetSocketLocation("Head"); 

		// Calculate the end location for the trace
		const FVector TraceEndB = EyesLocation + (ActorForwardVector * (VaultingMaxLenght + 5.0f));

		// Parameters for the trace
		FHitResult HitResult;

		// Perform the line trace
		bool bHitB = GetWorld()->LineTraceSingleByChannel(HitResult, EyesLocation, TraceEndB, ECC_Visibility);
		

		if (bHitB)
		{
			return false;
		}
		else
		{
			return true;
		}
	}

	return false;
}

void UVCS_MovementComponent_AC::DoVault(float Z)
{
	if(StateComponent)
	{
		if(StateComponent->IsStateValid(VaultState)){LastState = StateComponent->CurrentState;StateComponent->CurrentState = VaultState;}
	}
	
	ZForInterpolation = Z;
	// Play the montage with root motion
	PlayerRef->PlayAnimMontage(VaultMontage);
	UTimelineComponent* InterpTimeLine;
	InterpTimeLine = NewObject<UTimelineComponent>(this, FName("InterpTimeLine"));
	InterpTimeLine->RegisterComponent();
	

	// Add the interpolation float to the timeline
	FOnTimelineFloat FloatStaticFunc;
	FloatStaticFunc.BindUFunction(this, FName("AdjustHeightDuringMontage"));
	InterpTimeLine->AddInterpFloat(VaultCurveFloat, FloatStaticFunc);

	// Set the timeline length (you might need to adjust this based on your needs)
	InterpTimeLine->SetTimelineLength(VaultMontage->GetPlayLength()/3);

	// Play the timeline from the start
	InterpTimeLine->PlayFromStart();
}
void UVCS_MovementComponent_AC::AdjustHeightDuringMontage(float Alpha)
{
	// Interpolate the character's Z position smoothly
	FVector NewLocation = GetOwner()->GetActorLocation();
	NewLocation.Z = FMath::Lerp(GetOwner()->GetActorLocation().Z, ZForInterpolation + ZInterpMargin, Alpha);

	// Set the character's location
	GetOwner()->SetActorLocation(NewLocation);

	if(Alpha >= 0.9)
	{
		bEnableVaulting = true;
		if(!StateComponent)return;

		if(StateComponent->CurrentState.IsEqual(VaultState))StateComponent->CurrentState = LastState;
	}
}


// Called every frame
void UVCS_MovementComponent_AC::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
}

//////////////////////////
///Get Input Values/////////
/////////////////////

FVector2D UVCS_MovementComponent_AC::GetInputValues()
{
	return FVector2D(sideinput, forwardinput);
}

FVector2D UVCS_MovementComponent_AC::GetMouseInputValue()
{
	return MouseInput;
}

bool UVCS_MovementComponent_AC::GetSprint()
{
	return IsSprinting;
}



//////////////////////////
///INPUTS/////////
/////////////////////
void UVCS_MovementComponent_AC::MovementInput(const FInputActionValue& Value)
{
	if(!StateComponent->CanPerformAction(InputMove, "")){sideinput=0.f;forwardinput=0.f;return;}
	
	FVector2d input = Value.Get<FVector2d>();
	float multiply = 1.f;
	if(IsSprinting)
	{
		multiply *= 2;
	}
	
	sideinput = input.X * multiply;
	forwardinput = input.Y * multiply;

	InputOverride(InputMove, ETriggerEvent::Triggered, Value);
}

void UVCS_MovementComponent_AC::LookInput(const FInputActionValue& Value)
{
	if(!StateComponent->CanPerformAction(InputLook, "")){return;}
	
	const FVector2D LookValue = Value.Get<FVector2D>();
	MouseInput = FVector2d(LookValue.X * GetWorld()->DeltaTimeSeconds * sensitivity, LookValue.Y * GetWorld()->DeltaTimeSeconds * sensitivity * -1.f);
	if (LookValue.X != 0.f)
	{
		PlayerRef->AddControllerYawInput(LookValue.X * GetWorld()->DeltaTimeSeconds * sensitivity);
	}
 
	if (LookValue.Y != 0.f)
	{
		PlayerRef->AddControllerPitchInput(LookValue.Y * GetWorld()->DeltaTimeSeconds * sensitivity * -1.f);
	}
	
	InputOverride(InputLook, ETriggerEvent::Triggered, Value);
}

void UVCS_MovementComponent_AC::SprintInput(const FInputActionValue& Value)
{
	if(!StateComponent->CanPerformAction(InputSprint, "")){IsSprinting=false;return;}
	
	IsSprinting = Value.Get<bool>();

	if(IsSprinting)
		InputOverride(InputSprint, ETriggerEvent::Started, Value);
	else
	{
		InputOverride(InputSprint, ETriggerEvent::Completed, Value);
	}
}
