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

	UPROPERTY(Replicated)
	EGrabHand HoldingHand = EGrabHand::None; 

	UPROPERTY(Replicated)
	FTransform GrabRelativeTransform; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Throw")
	float ThrowStrengthMultiplier = 1.0f;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual bool TryClaim_Implementation(AVRPawnCustom* RequestingPawn, EGrabHand RequestingHand, const FTransform& GrabOffset) override;
	virtual bool ReleaseClaim_Implementation(AVRPawnCustom* RequestingPawn, const FVector& LinerarVelocity, const FVector& AngularVelocity) override;

	UFUNCTION(BlueprintPure, Category = "Networking:Grab")
	AVRPawnCustom* GetHoldingPawn() const
	{
		return HoldingPawn; 
	}

	UFUNCTION(BlueprintPure, Category = "Network:Grab")
	EGrabHand GetHoldingHand() const
	{
		return HoldingHand; 
	}

	UFUNCTION(BlueprintPure, Category = "Network:Grab")
	FTransform GetGrabRelativeTransform() const
	{
		return GrabRelativeTransform; 
	}


	UFUNCTION(BlueprintPure, Category = "Networking:Grab")
	bool IsNetworkHeld() const 
	{ 
		return bIsHeld; 
	}


	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;




};
