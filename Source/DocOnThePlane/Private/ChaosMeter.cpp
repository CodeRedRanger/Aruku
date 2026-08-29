// Fill out your copyright notice in the Description page of Project Settings.


#include "ChaosMeter.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "../DocOnThePlane.h"

void UChaosMeter::SetChaosMeterValue(float value)
{
	if (!ChaosMeterBar)
	{
		UE_LOG(Game, Error, TEXT("ChaosMeterBar is not a variable in ChaosMeter widget."));
		return; 
	}

	const float ClampedScore =
		FMath::Clamp(value, 0.0f, 100.0f);

	ChaosMeterBar->SetPercent(ClampedScore / 100.0f); 

	if (ChaosMeterValueText)
	{
		ChaosMeterValueText->SetText(FText::AsNumber(FMath::RoundToInt(ClampedScore)));
	}



}