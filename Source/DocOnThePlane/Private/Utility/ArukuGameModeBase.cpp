// Fill out your copyright notice in the Description page of Project Settings.


#include "Utility/ArukuGameModeBase.h"
#include "Actors/Characters/VRPawnCustom.h"
#include "Utility/MedGameInstance.h"
#include "../DocOnThePlane.h"

void AArukuGameModeBase::BeginPlay()
{
	UE_LOG(Game,Warning,TEXT("Custom GameMode BeginPlay: %s"),*GetName());

	Super::BeginPlay();

	UMedGameInstance* MedGameInstance = GetGameInstance<UMedGameInstance>();

	if (IsValid(MedGameInstance) && MedGameInstance->IsNetworkTravelInProgress())
	{
		UE_LOG(Game, Warning, TEXT("ServerTravel complete: clearing network travel flag."));
		MedGameInstance->SetNetworkTravelInProgress(false);
	}

	
}

void AArukuGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	if (!IsValid(NewPlayer))
	{
		return; 
	}

	UE_LOG(Game, Warning, TEXT("PostLogin: Player connect: %s, LocalController: %s."), *NewPlayer->GetName(), NewPlayer->IsLocalController() ? TEXT("YES") : TEXT("NO"));

	if (!NewPlayer->IsLocalController())
	{
		//Remote player joined

		for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
		{
			APlayerController* PlayerController = It->Get();

			if (!IsValid(PlayerController))
			{
				continue; 
			}	

			if (!PlayerController->IsLocalController())
			{
				continue; 
			}

			AVRPawnCustom* HostPawn = Cast<AVRPawnCustom>(PlayerController->GetPawn());

			if (IsValid(HostPawn))
			{
				HostPawn->HandleNetworkPlayerJoined(); 
				break; 
			}



		}
	}

}

void AArukuGameModeBase::Logout(AController* Exiting)
{
	UE_LOG(Game,Warning,TEXT("Custom GameMode Logout called."));

	if (IsValid(Exiting))
	{
		UE_LOG(Game, Warning, TEXT("Player leaving: %s"), *Exiting->GetName());
	}

	Super::Logout(Exiting); 

	UMedGameInstance* MedGameInstance = GetGameInstance<UMedGameInstance>(); 

	if (IsValid(MedGameInstance) && MedGameInstance->IsNetworkTravelInProgress())
	{
		UE_LOG(Game, Warning, TEXT("Logout occurred during Server Travel, ignoring player left message."));
		return; 
	}


	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		APlayerController* PlayerController = It->Get();

		if (!IsValid(PlayerController) ||  !PlayerController->IsLocalController())
		{
			continue;
		}

		AVRPawnCustom* HostPawn = Cast<AVRPawnCustom>(PlayerController->GetPawn()); 

		if (IsValid(HostPawn))
		{

			UE_LOG(Game,Warning,TEXT("Logout found host pawn %s. Calling HandleNetworkPlayerLeft."),*HostPawn->GetName());
			HostPawn->HandleNetworkPlayerLeft();
			break; 
		}
	}
}
