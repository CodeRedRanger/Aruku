// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ChaosMeter.generated.h"

/**
 * 
 */
UCLASS() 
class DOCONTHEPLANE_API UChaosMeter : public UUserWidget
{
	GENERATED_BODY()


public: 

	//UChaosMeter(const FObjectInitializer& ObjectInitializer); 

	UFUNCTION(BlueprintCallable)
	void SetChaosMeterValue(float value);

protected: 

	UPROPERTY(BlueprintReadOnly,meta = (BindWidget))
	class UProgressBar* ChaosMeterBar;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* ChaosMeterValueText;
	
};
