// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Utility/NetworkGrabbable.h"
#include "GrabbableBall.generated.h"

class AVRPawnCustom;

UCLASS()
class DOCONTHEPLANE_API AGrabbableBall : public AActor, public INetworkGrabbable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGrabbableBall();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(Replicated)
	bool bIsHeld = false; 

	//may move to public later if needed
	UPROPERTY(Replicated)
	AVRPawnCustom* HoldingPawn = nullptr;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual bool TryClaim_Implementation(AVRPawnCustom* RequestingPawn) override; 
	virtual bool ReleaseClaim_Implementation(AVRPawnCustom* RequestingPawn) override;

	UFUNCTION(BlueprintPure, Category = "Networking:Grab")
	AVRPawnCustom* GetHoldingPawn() const
	{
		return HoldingPawn; 
	}


	UFUNCTION(BlueprintPure, Category = "Networking:Grab")
	bool IsHeld() const 
	{ 
		return bIsHeld; 
	}


	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;




};
