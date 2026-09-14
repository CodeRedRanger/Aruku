// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ArukuGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class DOCONTHEPLANE_API AArukuGameModeBase : public AGameModeBase
{
	GENERATED_BODY()


protected:

	virtual void PostLogin(APlayerController* NewPlayer) override;
	
};
