// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/GrabbableBall.h"
#include "Net/UnrealNetwork.h"
#include "Actors/Characters/VRPawnCustom.h"
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

bool AGrabbableBall::TryClaim_Implementation(AVRPawnCustom* RequestingPawn)
{
	if (!HasAuthority() || !IsValid(RequestingPawn))
	{
		return false;
	}

	if (bIsHeld)
	{
		UE_LOG(Game, Warning, TEXT("Ball claim rejected, Ball: %s already held"), *GetName());

		return false;
	}

	bIsHeld = true;
	HoldingPawn = RequestingPawn;

	UE_LOG(Game, Warning, TEXT("Ball claimed: Ball %s, Holder %s"), *GetName(), *RequestingPawn->GetName());
	return true;
}

bool AGrabbableBall::ReleaseClaim_Implementation(AVRPawnCustom* RequestingPawn)
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

	UE_LOG(Game, Warning, TEXT("Ball released: Ball %s, Released by %s"), *GetName(), *RequestingPawn->GetName());
	return true; 

}


void AGrabbableBall::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps); 
	DOREPLIFETIME(AGrabbableBall, bIsHeld);
	DOREPLIFETIME(AGrabbableBall, HoldingPawn); 
}

