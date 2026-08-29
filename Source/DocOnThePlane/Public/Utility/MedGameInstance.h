// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MedGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class DOCONTHEPLANE_API UMedGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

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

protected:

	
	
private:
	UPROPERTY()
	float ChaosScore = 10.0f;
	bool bHasTriggeredWarning = false;
	bool bHasTriggeredCrowdNoise = false;

	


};
