// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/AudioComponent.h"
#include "VRPawnCustom.generated.h"

class UFlightAttendantWarning;
class AGrabbableBall; 

UCLASS()
class DOCONTHEPLANE_API AVRPawnCustom : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AVRPawnCustom();

	UFUNCTION(BlueprintCallable, Category = "Warning")
	void ShowFlightAttendantWarning();

	UFUNCTION(BlueprintCallable, Category = "Warning")
	void HideFlightAttendantWarning();

	UFUNCTION(BlueprintCallable, Category = "Warning")
	void ResetFlightAttendantWarning();


	//Networking
	UFUNCTION(BlueprintCallable, Category = "Network:Grab")
	void RequestNetworkGrab(AActor* GrabbableActor, EGrabHand GrabHand, const FTransform& GrabOffset); 


	UFUNCTION(BlueprintCallable, Category = "Network:Grab")
	void RequestNetworkRelease(AActor* GrabbableActor, const FVector& ReleaseLocation, const FRotator& ReleaseRotation, const FVector& LinearVelocity, const FVector& AngularVelocity);


protected:


	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Warning")
	TSubclassOf<UFlightAttendantWarning> FlightAttendantWarningClass;

	UPROPERTY(BlueprintReadOnly, Category = "Warning")
	class UWidgetComponent* FlightAttendantWarningComponent = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Warning")
	UFlightAttendantWarning* FlightAttendantWarningObject = nullptr;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	class UWidgetComponent* ChaosMeter = nullptr;

	UPROPERTY(Transient, BlueprintReadOnly, Category = "VR")
	class UCameraComponent* VRCameraReference = nullptr;

	
	//Networking

	UFUNCTION(Server, Reliable)
	void ServerRequestNetworkGrab(AActor* GrabbableActor, EGrabHand GrabHand, const FTransform& GrabOffset);

	UFUNCTION(Client, Reliable)
	void ClientRejectNetworkGrab();

	UFUNCTION(BlueprintImplementableEvent, Category = "Network:Grab")
	void HandleNetworkGrabRejected();

	UFUNCTION(Server, Reliable)
	void ServerRequestNetworkRelease(AActor* GrabbableActor, const FVector& ReleaseLocation, const FRotator& ReleaseRotation, const FVector& LinearVelocity, const FVector& AngularVelocity); 
	
	//Left hand
	UPROPERTY(ReplicatedUsing = OnRep_LeftHandTransform)
	FTransform ReplicatedLeftHandTransform;

	UFUNCTION()
	void OnRep_LeftHandTransform(); 

	UFUNCTION(Server, Unreliable)
	void ServerUpdateLeftHandTransform(const FTransform& NewLeftHandTransform); 

	UFUNCTION(BlueprintCallable, Category = Networking)
	void UpdateLocalLeftHandTransform(const FTransform& NewLeftHandTransform); 

	
	UPROPERTY(ReplicatedUsing = OnRep_RightHandTransform)
	FTransform ReplicatedRightHandTransform; 
	
	UFUNCTION()
	void OnRep_RightHandTransform(); 

	UFUNCTION(Server, Unreliable)
	void ServerUpdateRightHandTransform(const FTransform& NewRightHandTransform); 

	UFUNCTION(BlueprintCallable, Category = Networking)
	void UpdateLocalRightHandTransform(const FTransform& NewRightHandTransform); 
	

	//Head
	UPROPERTY(ReplicatedUsing = OnRep_HeadTransform)
	FTransform ReplicatedHeadTransform;

	UFUNCTION()
	void OnRep_HeadTransform(); 

	UFUNCTION(Server, Unreliable)
	void ServerUpdateHeadTransform(const FTransform& NewHeadTransform); 

	UFUNCTION(BlueprintCallable, Category = Networking)
	void UpdateLocalHeadTransform(const FTransform& NewHeadTransform);

	//Teleport
	UFUNCTION(Server, Reliable)
	void ServerTeleportPawn(const FVector& NewLocation, const FRotator& NewRotation);

	UFUNCTION(Server, Reliable)
	void ServerUpdatePawnRotation(const FRotator& NewRotation); 

	//Hand animations

	//Left hand animations

	UPROPERTY(Replicated)
	float ReplicatedLeftGrasp = 0.0f; 

	UFUNCTION(Server, Unreliable)
	void ServerUpdateLeftGrasp(float NewValue); 


	UPROPERTY(Replicated)
	float ReplicatedLeftIndexCurl = 0.0f;

	UPROPERTY(Replicated)
	float ReplicatedLeftPoint = 0.0f;

	UPROPERTY(Replicated)
	float ReplicatedLeftThumbUp = 0.0f;

	UFUNCTION(Server, Unreliable)
	void ServerUpdateLeftIndexCurl(float NewValue);

	UFUNCTION(Server, Unreliable)
	void ServerUpdateLeftPoint(float NewValue);

	UFUNCTION(Server, Unreliable)
	void ServerUpdateLeftThumbUp(float NewValue);


	//Right hand animations

	UPROPERTY(Replicated) 
	float ReplicatedRightGrasp = 0.0f; 

	UFUNCTION(Server, Unreliable)
	void ServerUpdateRightGrasp(float NewValue); 

	UPROPERTY(Replicated)
	float ReplicatedRightIndexCurl = 0.0f;

	UPROPERTY(Replicated)
	float ReplicatedRightPoint = 0.0f;

	UPROPERTY(Replicated)
	float ReplicatedRightThumbUp = 0.0f;

	UFUNCTION(Server, Unreliable)
	void ServerUpdateRightIndexCurl(float NewValue);

	UFUNCTION(Server, Unreliable)
	void ServerUpdateRightPoint(float NewValue);

	UFUNCTION(Server, Unreliable)
	void ServerUpdateRightThumbUp(float NewValue);


public:

	//Networking: Hand animations

	//Left hand animations
	UFUNCTION(BlueprintCallable, Category = "Network:HandAnimations")
	void UpdateLocalLeftGrasp(float NewValue);

	UFUNCTION(BlueprintPure, Category = "Network:HandAnimations")
	float GetReplicatedLeftGrasp() const
	{
		return ReplicatedLeftGrasp; 
	}

	UFUNCTION(BlueprintCallable, Category = "Network:HandAnimations")
	void UpdateLocalLeftIndexCurl(float NewValue);

	UFUNCTION(BlueprintCallable, Category = "Network:HandAnimations")
	void UpdateLocalLeftPoint(float NewValue);

	UFUNCTION(BlueprintCallable, Category = "Network:HandAnimations")
	void UpdateLocalLeftThumbUp(float NewValue);

	UFUNCTION(BlueprintPure, Category = "Network:HandAnimations")
	float GetReplicatedLeftIndexCurl() const
	{
		return ReplicatedLeftIndexCurl;
	}

	UFUNCTION(BlueprintPure, Category = "Network:HandAnimations")
	float GetReplicatedLeftPoint() const
	{
		return ReplicatedLeftPoint;
	}

	UFUNCTION(BlueprintPure, Category = "Network:HandAnimations")
	float GetReplicatedLeftThumbUp() const
	{
		return ReplicatedLeftThumbUp;
	}


	//Right hand animations
	UFUNCTION(BlueprintCallable, Category = "Network:HandAnimations")
	void UpdateLocalRightGrasp(float NewValue); 

	UFUNCTION(BlueprintPure, Category = "Network:HandAnimations")
	float GetReplicatedRightGrasp() const
	{
		return ReplicatedRightGrasp; 
	}

	UFUNCTION(BlueprintCallable, Category = "Network:HandAnimations")
	void UpdateLocalRightIndexCurl(float NewValue);

	UFUNCTION(BlueprintCallable, Category = "Network:HandAnimations")
	void UpdateLocalRightPoint(float NewValue);

	UFUNCTION(BlueprintCallable, Category = "Network:HandAnimations")
	void UpdateLocalRightThumbUp(float NewValue);

	UFUNCTION(BlueprintPure, Category = "Network:HandAnimations")
	float GetReplicatedRightIndexCurl() const
	{
		return ReplicatedRightIndexCurl;
	}

	UFUNCTION(BlueprintPure, Category = "Network:HandAnimations")
	float GetReplicatedRightPoint() const
	{
		return ReplicatedRightPoint;
	}

	UFUNCTION(BlueprintPure, Category = "Network:HandAnimations")
	float GetReplicatedRightThumbUp() const
	{
		return ReplicatedRightThumbUp;
	}


	//Networking
	UFUNCTION(BlueprintCallable, Category = Networking)
	void NotifyServerOfRotation(const FRotator& NewRotation); 

	UFUNCTION(BlueprintCallable, Category = Networking)
	void NotifyServerOfTeleport(const FVector& NewLocation, const FRotator& NewRotation); 
	 
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintPure, Category = Networking)
	FTransform GetReplicatedHeadTransform() const
	{
		return ReplicatedHeadTransform; 
	}

	UFUNCTION(BlueprintPure, Category = Networking)
	FTransform GetReplicatedLeftHandTransform() const
	{
		return ReplicatedLeftHandTransform; 
	}

	UFUNCTION(BlueprintPure, Category = Networking)
	FTransform GetReplicatedRightHandTransform() const
	{
		return ReplicatedRightHandTransform; 
	}

	//Networking: Grab
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Network:Grab")
	FTransform GetNetworkGrabHandTransform(EGrabHand GrabHand) const; 

	//Debug functions to test network replication without a second player.
	UFUNCTION(BlueprintCallable, Category = "Network Debug")
	void DebugMoveRight(); 

	UFUNCTION(Server, Reliable)
	void ServerDebugMoveRight(); 

	UFUNCTION(BlueprintCallable, Category = "Network Debug")
	void DebugPrintPawnLocation(); 

	UFUNCTION(BlueprintCallable, Category = "Network Debug")
	void DebugRequestGrab(AActor* GrabbableActor, EGrabHand GrabHand, const FTransform& GrabOffset);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//Below is a test to see if begin play being accessed by BP wrapper
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<class UUserWidget> ChaosMeterClass;

	//can move to protected since using getter
	UPROPERTY()
	class UChaosMeter* ChaosMeterObject = nullptr;

	UChaosMeter* GetChaosMeterObject() const { return ChaosMeterObject; }	

	UPROPERTY(BlueprintReadOnly)
	UAudioComponent* CrowdClamorAudioComponent;


private:

	UPROPERTY()
	class UMedGameInstance* MedGameInstanceRefCode = nullptr;

	UFUNCTION()
	void HandleFlightAttendantWarningOK();

	bool bFlightAttendantWarningAcknowledged = false; 



	

};
