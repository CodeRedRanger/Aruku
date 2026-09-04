// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/AudioComponent.h"
#include "VRPawnCustom.generated.h"

class UFlightAttendantWarning;

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
	void ServerTeleportPawn(const FVector& NewLocation); 


public:

	UFUNCTION(BlueprintCallable, Category = Networking)
	void NotifyServerOfTeleport(const FVector& NewLocation); 
	 
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

	UFUNCTION(BlueprintCallable, Category = "Network Debug")
	void DebugMoveRight(); 

	UFUNCTION(Server, Reliable)
	void ServerDebugMoveRight(); 

	UFUNCTION(BlueprintCallable, Category = "Network Debug")
	void DebugPrintPawnLocation(); 

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
