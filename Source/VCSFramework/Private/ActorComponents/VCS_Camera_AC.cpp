// Copyright 2023 Diogo Ventura, All Rights Reserved.


#include "ActorComponents/VCS_Camera_AC.h"

#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"

UVCS_Camera_AC::UVCS_Camera_AC(): Pawn(nullptr), SpringArm(nullptr), Current_FieldOfView(0) 
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
}


void UVCS_Camera_AC::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(Pawn ==	NULL) //Fix NULLPointer error
		return;
	
	PhysicsCalculus();
	Fov();
}

void UVCS_Camera_AC::BeginPlay()
{
	Super::BeginPlay();

	
	//SetComponentTickEnabled(false);
	Pawn = Cast<APawn>(GetOwner());
	USpringArmComponent* T_SpringArm = Pawn->FindComponentByClass<USpringArmComponent>();
	if (IsValid(T_SpringArm))
	{
		
		SpringArm = T_SpringArm;
		if (GetAttachParent() != SpringArm)
		{
			DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepRelative, EDetachmentRule::KeepRelative, EDetachmentRule::KeepRelative, true));
			AttachToComponent(SpringArm, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, true));
		}

		ResetRelativeTransform();
		bUsePawnControlRotation = false;
		SpringArm->bUsePawnControlRotation = true;
		Current_SocketOffSet = SpringArm->SocketOffset;
		Current_FieldOfView = FieldOfView;
		SetComponentTickEnabled(true);
	}
}

void UVCS_Camera_AC::PhysicsCalculus()
{
	/// <summary>
	/// Velocity Based Camera Torque using Math Equations
	/// </summary>

	FVector PawnVelocity = Pawn->GetVelocity();
	FRotator PawnRotation = Pawn->GetActorRotation();


	//RightDotProduct
	float DotRight = FVector::DotProduct(PawnVelocity, GetRightVector());

	//UpDotProduct
	float DotUP = FVector::DotProduct(PawnVelocity, GetUpVector());

	//ZAxisDotProduct
	FVector X, Y, Z, WX, WY, WZ;
	UKismetMathLibrary::GetAxes(PawnRotation, X, Y, Z);
	UKismetMathLibrary::GetAxes(GetComponentRotation(), WX, WY, WZ);
	float zDot = FVector::DotProduct(Z, WZ);


	//Player-Camera Right Vector Calculus
	float PRightVector = ((TorqueAmount * (DotRight / VelocityRange)) * zDot);

	//Player-Camera Up Vector Calculus
	float PUpVector = (TorqueAmount * (DotUP / VelocityRange));

	//Right Vector Calculus
	float RightVector = (TorqueAmount * (DotRight / VelocityRange));

	//Clamp Values Based on Custom Variable and Make A rotator

	PRightVector = FMath::Clamp(PRightVector, MaxRoll * -1.0f, MaxRoll);
	PUpVector = FMath::Clamp(PUpVector, MaxPitch + -1.0f, MaxPitch);
	RightVector = FMath::Clamp(RightVector, MaxYaw * -1.0f, MaxYaw);

	//Get current transform
	FTransform CurrentTransform = GetRelativeTransform();

	FRotator Rotation = FMath::RInterpTo(CurrentTransform.Rotator(), FRotator(PUpVector, RightVector, PRightVector), SDeltaTime, InterpSpeed);
	FTransform NewTransform = FTransform(
		FRotator(Rotation.Pitch, Rotation.Yaw, Rotation.Roll),
		FVector(0.0f, 0.0f, 0.0f),
		FVector(1.0f, 1.0f, 1.0f)
	);

	SetRelativeTransform(NewTransform);
	//SetRelativeRotation(Rotation);

	///////////////
	///Velocity Based Camera Pan Using Calculus
	//////////////

	PawnVelocity = Pawn->GetVelocity();
	PawnRotation = Pawn->GetActorRotation();

	// Dot Products
	float ForwardDot = FVector::DotProduct(PawnVelocity, GetForwardVector());
	float RightDot = FVector::DotProduct(PawnVelocity, GetRightVector());
	float PureUPDot = FVector::DotProduct(FVector(0.0f, 0.0f, PawnVelocity.Z), GetUpVector());

	UKismetMathLibrary::GetAxes(PawnRotation, X, Y, Z);
	UKismetMathLibrary::GetAxes(GetComponentRotation(), WX, WY, WZ);
	float ZDot = FVector::DotProduct(Z, WZ);

	float ForwardVec = ((PanAmount * (ForwardDot / VelocityRange)) * ZDot);
	float RightVec = (PanAmount * (RightDot / VelocityRange));
	float PureUPVec = ((PanAmount * (PureUPDot / VelocityRange)) * ZDot);

	ForwardVec = FMath::Clamp(ForwardVec, MaxPanX * -1.0f, MaxPanX);
	RightVec = FMath::Clamp(RightVec, MaxPanY * -1.0f, MaxPanY);
	PureUPVec = FMath::Clamp(PureUPVec, MaxPanZ * -1.0f, MaxPanZ);

	FVector ClampedVector = FVector(Current_SocketOffSet.X + ForwardVec, Current_SocketOffSet.Y + RightVec, Current_SocketOffSet.Z + PureUPVec);
	FVector InterpVec = FMath::VInterpTo(SpringArm->SocketOffset, ClampedVector, SDeltaTime, InterpSpeed);
	SpringArm->SocketOffset = InterpVec;

}

void UVCS_Camera_AC::Fov()
{
	
	FVector Velocity = Pawn->GetVelocity();
	float VelocityxVec = FVector::DotProduct(Velocity, GetForwardVector());
	float FOVProduct = Current_FieldOfView + ((VelocityxVec / VelocityRange) * FieldOfViewAmount);
	FOVProduct = FMath::Clamp(FOVProduct, MaxFieldOfView * -1.0f, MaxFieldOfView);

	float InterpValue = FMath::FInterpTo(FieldOfView, FOVProduct, SDeltaTime, InterpSpeed);
	SetFieldOfView(InterpValue);
}