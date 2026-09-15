// Fill out your copyright notice in the Description page of Project Settings.


#include "Utility/MedGameInstance.h"
#include "Engine/Engine.h"
#include "Engine/NetDriver.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/PlayerController.h"
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
	const FString CleanAddress = Address.TrimStartAndEnd(); 

	if (!IsValidIPv4Address(CleanAddress))
	{
		UE_LOG(Game, Warning, TEXT("JoinGameByIP failed: Address is not a valid IPv4 address: %s."), *CleanAddress);
		return false; 
	}

	APlayerController* PlayerController = GetFirstLocalPlayerController(); 

	if (!IsValid(PlayerController))
	{
		UE_LOG(Game, Error, TEXT("JoinGameByIP failed: No local PlayerController."));
		return false; 
	}

	FString AddressWithPort = Address;

	if (!AddressWithPort.Contains(TEXT(":")))
	{
		AddressWithPort += TEXT(":7777"); // Default port
	}

	UE_LOG(Game, Warning, TEXT("JoinGameByIP: Attempting to connect to %s"), *Address);

	PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
	
	return true; 
}

void UMedGameInstance::Init()
{
	Super::Init();

	if (GEngine)
	{
		GEngine->OnNetworkFailure().AddUObject(this, &UMedGameInstance::HandleNetworkFailure);
	}
}

void UMedGameInstance::Shutdown()
{
	if (GEngine)
	{
		GEngine->OnNetworkFailure().RemoveAll(this); 
	}

	Super::Shutdown();
}


void UMedGameInstance::LeaveNetworkGame()
{
	UWorld* World = GetWorld(); 

	if (!IsValid(World))
	{
		return; 
	}

	ResetNetworkStateForLeave(); 

	const ENetMode NetMode = World->GetNetMode(); 

	if (NetMode == NM_ListenServer)
	{
		//Host leaving
		if (AGameModeBase* GameMode = World->GetAuthGameMode())
		{
			GameMode->ReturnToMainMenuHost(); 
		}

		return;

	}

	if (NetMode == NM_Client)
	{

		//Client leaving
		if (APlayerController* PlayerController = GetFirstLocalPlayerController())
		{
			PlayerController->ClientTravel(TEXT("/Game/Maps/L_MainMenu"), ETravelType::TRAVEL_Absolute);
		}
	}

}

bool UMedGameInstance::IsValidIPv4Address(const FString& Address) const
{

	TArray<FString> Parts;
	Address.ParseIntoArray(Parts, TEXT("."), true);

	if (Parts.Num() != 4)
	{
		UE_LOG(Game, Warning, TEXT("IsValidIPv4Address: Address does not have 4 parts."));
		return false;
	}

	for (const FString& Part : Parts)
	{
		if (Part.IsEmpty())
		{
			return false;
		}

		//Check that characters are digits
		for (TCHAR Character : Part)
		{
			if (!FChar::IsDigit(Character))
			{
				return false;
			}
		}

		const int32 Value = FCString::Atoi(*Part);

		if (Value < 0 || Value > 255)
		{
			return false;
		}

	}

	return true;
}

void UMedGameInstance::HandleNetworkFailure(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type FailureType, const FString& ErrorString)
{
	UE_LOG(Game, Error, TEXT("Network failure: Type: %d, Error: %s"), static_cast<int32>(FailureType), *ErrorString);
		
	HandleNetworkFailureState(ErrorString);

	//no longer needed since handle network failure state covers this
	//OnNetworkJoinFailed.Broadcast(TEXT("Could not connect to host.")); //this will set network state to join failed, so not good if host leaves

	

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

