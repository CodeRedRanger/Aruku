// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ConversationComponent.generated.h"


UCLASS( Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DOCONTHEPLANE_API UConversationComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UConversationComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Chaos")
	void RespondToChaos(float ChaosScoreIn, bool GameIsOver);

public:	
	// Called every frame
	//virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//chaoschange, already in conversation node and is passed to calculate chaos score function

	//chaosscore, will later move to game instance, because needs to carry over between NPCs
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chaos")
	//float ChaosScore = 10.0f;

	//calculate chaos score
	UFUNCTION(BlueprintCallable, Category = "Chaos")
	float CalculateChaosScore(float ChaosChange);

	//display chaos score
	UFUNCTION(BlueprintCallable, Category = "Chaos")
	void UpdateChaosMeter(float ChaosScoreCurrent);		


	UPROPERTY(BlueprintReadOnly, Category = "Chaos")
	class UMedGameInstance* MedGameInstanceRefCode = nullptr;


private:
	UPROPERTY()
	class AVRPawnCustom* VRPawn = nullptr;

	UPROPERTY()
	class UChaosMeter* ChaosMeter = nullptr;


		

};
