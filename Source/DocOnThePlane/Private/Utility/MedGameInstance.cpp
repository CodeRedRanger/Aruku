// Fill out your copyright notice in the Description page of Project Settings.


#include "Utility/MedGameInstance.h"
#include "../DocOnThePlane.h"


bool UMedGameInstance::HostGame()
{
	const bool bStartedListening = EnableListenServer(true, 0);

	if (bStartedListening)
	{
		UE_LOG(Game, Log, TEXT("HostGame SUCCESS: Main menu is now a listen server."));
	}
	else
	{
		UE_LOG(Game, Error, TEXT("HostGame FAILED: Could not start listen server."));
	}

	return bStartedListening; 
}

bool UMedGameInstance::JoinGameByIP(const FString& Address)
{
	if (Address.IsEmpty())
	{
		UE_LOG(Game, Warning, TEXT("JoinGameByIP failed: Address is empty."));
		return false; 
	}

	APlayerController* PlayerController = GetFirstLocalPlayerController(); 

	if (!IsValid(PlayerController))
	{
		UE_LOG(Game, Error, TEXT("JoinGameByIP failed: No local PlayerController."));
		return false; 
	}

	UE_LOG(Game, Warning, TEXT("JoinGameByIP: Attempting to connect to %s"), *Address);

	PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
	
	return true; 
}

//CAN REMOVE ALL BELOW
float UMedGameInstance::AddChaos(float ChaosChange)
{
	ChaosScore = FMath::Clamp(
		ChaosScore + ChaosChange,
		0.0f,
		100.0f
	);

	return ChaosScore;
}

float UMedGameInstance::GetChaosScore() const
{
	return ChaosScore;
}

void UMedGameInstance::ResetChaosScore()
{
	ChaosScore = 10.0f;
	bChaosGameOver = false;
}

void UMedGameInstance::HandleChaosScore(float ChaosScoreForBranch)
{
	//have to make sure each is only played once until a change in category
	//use bool that set true inside of branch and set false inside of another branch

	if (ChaosScoreForBranch >= 80.0f)
	{
		UE_LOG(Game, Warning, TEXT("MedGameInstance: ChaosScoreForBranch is greater than or equal to 80. Triggering game over."));
		//trigger game over, do if check after this function and if 80 or above -> game over sequence.
		bChaosGameOver = true;
		bStartCrowdClamor = false;

		bHasTriggeredWarning = false;
		bHasTriggeredCrowdNoise = false;
		bHasTriggeredNormal = false;

	}
	else if (ChaosScoreForBranch >= 50.0f && ChaosScoreForBranch < 80.0f && !bHasTriggeredCrowdNoise && !bHasTriggeredWarning)
	{
		UE_LOG(Game, Warning, TEXT("MedGameInstance: ChaosScoreForBranch is between 50 and 80. Triggering warning."));
		//trigger warning and crowd clamoring
		bStartCrowdClamor = true;

		bHasTriggeredWarning = true;
		bHasTriggeredCrowdNoise = true;
		bHasTriggeredNormal = false;
		
	}
	else if (ChaosScoreForBranch < 50.0f && !bHasTriggeredNormal)
	{
		UE_LOG(Game, Log, TEXT("MedGameInstance: ChaosScoreForBranch is %f."), ChaosScoreForBranch);
		//stop crowd clamoring 
		bStartCrowdClamor = false;

		bHasTriggeredWarning = false;
		bHasTriggeredCrowdNoise = false;
		bHasTriggeredNormal = true;
	}
}

//END REMOVE