// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/ObjectLabelWidget.h"
#include "Components/TextBlock.h"
#include "../DocOnThePlane.h"


void UObjectLabelWidget::SetLabelText(const FText& NewText)
{

	if (!LabelText)
	{
		UE_LOG(Game, Error,TEXT("ObjectLabelWidget: LabelText is NULL."));
		return;
	}

	
	UE_LOG(Game,Warning,TEXT("Setting object label to: %s"),*NewText.ToString());
	LabelText->SetText(NewText); 
	
}
