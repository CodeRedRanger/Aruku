// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MedGameInstance.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FNetworkJoinFailedSignature, const FString&, ErrorMessage);
UCLASS()
class DOCONTHEPLANE_API UMedGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "Networking:Travel")
	void StartNetworkGame(); 

	UFUNCTION(BlueprintCallable, Category = "Networking:Lobby")
	bool HostGame(); 

	UFUNCTION(BlueprintCallable, Category = "Networking:Lobby")
	bool JoinGameByIP(const FString& Address); 

	virtual void Init() override;
	virtual void Shutdown() override;
	
	UPROPERTY(BlueprintAssignable, Category = "Networking:Lobby")
	FNetworkJoinFailedSignature OnNetworkJoinFailed;

	UFUNCTION(BlueprintCallable, Category = "Networking:Lobby")
	void LeaveNetworkGame(); 

	UFUNCTION(BlueprintPure, Category = "Network: Travel")
	bool IsNetworkTravelInProgress() const
	{
		return bNetworkTravelInProgress;
	}

	void SetNetworkTravelInProgress(bool bInProgress)
	{
		bNetworkTravelInProgress = bInProgress; 
	}

protected:
	UFUNCTION(BlueprintImplementableEvent, Category = "Networking:Lobby")
	void ResetNetworkStateForLeave(); 

	UFUNCTION(BlueprintImplementableEvent, Category = "Networking:Lobby")
	void HandleNetworkFailureState(const FString& ErrorMessage); 

private:
	bool IsValidIPv4Address(const FString& Address) const;
	void HandleNetworkFailure(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type FailureType, const FString& ErrorString);

	bool bNetworkTravelInProgress = false; 

public: 
	//All of this can be removed
	UFUNCTION(BlueprintCallable, Category = "Chaos")
	float AddChaos(float ChaosChange);

	UFUNCTION(BlueprintPure, Category = "Chaos")
	float GetChaosScore() const;

	UFUNCTION(BlueprintCallable, Category = "Chaos")
	void ResetChaosScore(); 

	UFUNCTION(BlueprintCallable, Category = "Chaos")
	void HandleChaosScore(float ChaosScoreForBranch);

	UPROPERTY()
	bool bStartCrowdClamor = false;

	UPROPERTY()
	bool bChaosGameOver = false;

	UPROPERTY()
	bool bHasTriggeredNormal = false;

	UPROPERTY(EditDefaultsOnly, Category = "Audio")
	USoundBase* CrowdClamorSound;
	//end remove



	
	
private:

	//All of this can be removed
	UPROPERTY()
	float ChaosScore = 10.0f;
	bool bHasTriggeredWarning = false;
	bool bHasTriggeredCrowdNoise = false;
	//end remove



	


};
