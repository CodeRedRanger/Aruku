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
