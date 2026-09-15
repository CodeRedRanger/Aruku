// Fill out your copyright notice in the Description page of Project Settings.


#include "Utility/ArukuGameModeBase.h"
#include "Actors/Characters/VRPawnCustom.h"
#include "../DocOnThePlane.h"

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
	if (IsValid(Exiting))
	{
		UE_LOG(Game, Warning, TEXT("Player leaving: %s"), *Exiting->GetName());
	}

	Super::Logout(Exiting); 

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
			HostPawn->HandleNetworkPlayerLeft();
			break; 
		}
	}
}
