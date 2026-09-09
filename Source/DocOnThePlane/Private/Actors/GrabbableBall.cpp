// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/GrabbableBall.h"
#include "Net/UnrealNetwork.h"
#include "Actors/Characters/VRPawnCustom.h"
#include "Components/PrimitiveComponent.h"
#include "../DocOnThePlane.h"


// Sets default values
AGrabbableBall::AGrabbableBall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	SetReplicateMovement(true); 

}

// Called when the game starts or when spawned
void AGrabbableBall::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGrabbableBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool AGrabbableBall::TryClaim_Implementation(AVRPawnCustom* RequestingPawn, EGrabHand RequestingHand, const FTransform& GrabOffset)
{
	if (!HasAuthority() || !IsValid(RequestingPawn) || RequestingHand == EGrabHand::None)
	{
		return false;
	}

	
	if (bIsHeld)
	{

		if (HoldingPawn == RequestingPawn)
		{
			HoldingHand = RequestingHand;
			GrabRelativeTransform = GrabOffset;

			const TCHAR* HandName = RequestingHand == EGrabHand::Left ? TEXT("Left") : TEXT("Right");
			
			UE_LOG(Game, Warning, TEXT("Ball hand changed: Ball %s, Holder %s, Hand %s"), *GetName(), *RequestingPawn->GetName(), HandName);
			
			return true;
		}
		

		UE_LOG(Game, Warning, TEXT("Ball claim rejected, Ball: %s already held"), *GetName());

		return false;
	}

	UPrimitiveComponent* PhysicsComponent = FindComponentByClass<UPrimitiveComponent>(); 
	if (IsValid(PhysicsComponent))
	{
		PhysicsComponent->SetPhysicsLinearVelocity(FVector::ZeroVector); 
		PhysicsComponent->SetPhysicsAngularVelocityInDegrees(FVector::ZeroVector);
		PhysicsComponent->SetSimulatePhysics(false);
	}

	bIsHeld = true;
	HoldingPawn = RequestingPawn;
	HoldingHand = RequestingHand;
	GrabRelativeTransform = GrabOffset;

	const TCHAR* HandName = RequestingHand == EGrabHand::Left ? TEXT("Left") : TEXT("Right"); 

	UE_LOG(Game, Warning, TEXT("Ball claimed: Ball %s, Holder %s, Hand %s"), *GetName(), *RequestingPawn->GetName(), HandName);
	return true;
}

bool AGrabbableBall::ReleaseClaim_Implementation(AVRPawnCustom* RequestingPawn, const FVector& LinearVelocity, const FVector& AngularVelocity)
{
	if (!HasAuthority() || !IsValid(RequestingPawn))
	{
		return false;
	}

	if (!bIsHeld)
	{
		return false;
	}

	if (HoldingPawn != RequestingPawn)
	{
		return false; 
	}

	bIsHeld = false;
	HoldingPawn = nullptr;
	HoldingHand = EGrabHand::None; 

	UPrimitiveComponent* PhysicsComponent = FindComponentByClass<UPrimitiveComponent>(); 

	if (IsValid(PhysicsComponent))
	{
		PhysicsComponent->SetSimulatePhysics(true);

		PhysicsComponent->SetPhysicsLinearVelocity(LinearVelocity * ThrowStrengthMultiplier);

		PhysicsComponent->SetPhysicsAngularVelocityInDegrees(AngularVelocity); 
	}


	UE_LOG(Game, Warning, TEXT("Ball released: Ball %s, Released by %s,"
		"LinearVel %s, AngularVel %s"), *GetName(), *RequestingPawn->GetName(),
		*LinearVelocity.ToString(), *AngularVelocity.ToString());
	return true; 

}


void AGrabbableBall::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps); 
	DOREPLIFETIME(AGrabbableBall, bIsHeld);
	DOREPLIFETIME(AGrabbableBall, HoldingPawn); 
	DOREPLIFETIME(AGrabbableBall, HoldingHand); 
	DOREPLIFETIME(AGrabbableBall, GrabRelativeTransform); 
}

