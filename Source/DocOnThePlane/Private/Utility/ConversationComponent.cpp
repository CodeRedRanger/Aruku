// Fill out your copyright notice in the Description page of Project Settings.


#include "Utility/ConversationComponent.h"
#include "Actors/Characters/VRPawnCustom.h"
#include "ChaosMeter.h"
#include "Utility/MedGameInstance.h"
#include "GameFramework/PlayerController.h"
#include "../DocOnThePlane.h"



// Sets default values for this component's properties
UConversationComponent::UConversationComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UConversationComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	

}

void UConversationComponent::RespondToChaos(float ChaosScoreIn, bool GameIsOver)
{
	UWorld* World = GetWorld();

	if (!World)
	{
		UE_LOG(Game, Error, TEXT("ConversationComponent: World is invalid."));
		return;
	}

	MedGameInstanceRefCode = Cast<UMedGameInstance>(World->GetGameInstance());
	if (!MedGameInstanceRefCode)
	{
		UE_LOG(Game, Error, TEXT("ConversationComponent: MedGameInstanceRefCode is invalid."));
		return;
	}
	MedGameInstanceRefCode->HandleChaosScore(ChaosScoreIn);

	if (MedGameInstanceRefCode->bHasTriggeredNormal)
	{
		VRPawn->ResetFlightAttendantWarning();
	}



	APlayerController* PlayerController = World->GetFirstPlayerController();

	if (!PlayerController)
	{
		UE_LOG(Game, Error, TEXT("ConversationComponent: PlayerController is invalid."));
		return;
	}

	VRPawn = Cast<AVRPawnCustom>(PlayerController->GetPawn());

	if (!VRPawn)
	{
		UE_LOG(Game, Error, TEXT("ConversationComponent: VRPawn is invalid."));
		return;
	}

	if(MedGameInstanceRefCode->bStartCrowdClamor && !GameIsOver)
	{
		UE_LOG(Game, Log, TEXT("ConversationComponent: bStartCrowdClamor is true. Starting crowd clamor."));
		//start crowd clamor
		//Pawn will handle this
		if(!VRPawn->CrowdClamorAudioComponent->IsPlaying())
		{
			VRPawn->CrowdClamorAudioComponent->Play();
		}
		//bring up flight attendant warning
		VRPawn->ShowFlightAttendantWarning();
	}
	else if (!MedGameInstanceRefCode->bChaosGameOver)
	{

		//handles chaos meter going to normal and also any ending except for one caused by high chaos score
		UE_LOG(Game, Log, TEXT("ConversationComponent: bStartCrowdClamor is false. Stopping crowd clamor."));
		//stop crowd clamor
		//Pawn will handle this
		if (VRPawn->CrowdClamorAudioComponent && VRPawn->CrowdClamorAudioComponent->IsPlaying())
		{
			VRPawn->CrowdClamorAudioComponent->Stop();
		}
		
		

	}
	else
	{
		//handle game over due to chaos
		UE_LOG(Game, Log, TEXT("ConversationComponent: Game over due to chaos! bStartCrowdClamor is false. Stopping crowd clamor."));
		//stop crowd clamor
		//Pawn will handle this
		if (VRPawn->CrowdClamorAudioComponent && VRPawn->CrowdClamorAudioComponent->IsPlaying())
		{
			VRPawn->CrowdClamorAudioComponent->Stop();
		}

		VRPawn->HideFlightAttendantWarning();

		//bring up chaos ending
		//need to design this.
	}

}

float UConversationComponent::CalculateChaosScore(float ChaosChange)
{
	UWorld* World = GetWorld();

	if (!World)
	{
		UE_LOG(Game, Error, TEXT("ConversationComponent: World is invalid."));
		return 0.0f;
	}

	//get the game instance
	MedGameInstanceRefCode = Cast<UMedGameInstance>(World->GetGameInstance());

	if (!MedGameInstanceRefCode)
	{
		UE_LOG(Game, Error, TEXT("ConversationComponent: MedGameInstanceRefCode	is invalid.")); 
		return 0.0f;
	}

	MedGameInstanceRefCode->AddChaos(ChaosChange);

	UpdateChaosMeter(MedGameInstanceRefCode->GetChaosScore());

	return MedGameInstanceRefCode	->GetChaosScore();
}

void UConversationComponent::UpdateChaosMeter(float ChaosScoreCurrent)
{
	UWorld* World = GetWorld();

	if (!World)
	{
		UE_LOG(Game, Error, TEXT("ConversationComponent: World is invalid."));
		return;
	}


	//get the controller/pawn

	APlayerController* PlayerController = World->GetFirstPlayerController();

	if(!PlayerController)
	{
		UE_LOG(Game, Error, TEXT("ConversationComponent: PlayerController is invalid."));
		return;
	}
	
	VRPawn = Cast<AVRPawnCustom>(PlayerController->GetPawn());
	
	if(!VRPawn)
	{
		UE_LOG(Game, Error, TEXT("ConversationComponent: VRPawn is invalid."));
		return;
	}

	//cast to VRPawnCustom
	//as VRPawnCustom, get chaos meter widget, get user widget object
	
	ChaosMeter = VRPawn->GetChaosMeterObject();

	if(!ChaosMeter)
	{
		UE_LOG(Game, Error, TEXT("ConversationComponent: ChaosMeter is invalid."));
		return;
	}

	//cast to a WBP_chaos meter (or just to a chaos meter)
	//set the value of the chaos meter (function inside of chaos meter)

	ChaosMeter->SetChaosMeterValue(ChaosScoreCurrent);





}



// Called every frame
/*
void UConversationComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}
*/
