// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/FlightAttendantWarning.h"
#include "Components/Button.h"

void UFlightAttendantWarning::NativeConstruct()
{
	Super::NativeConstruct();
	if (!WarningOKButton)
	{
		UE_LOG(LogTemp, Error, TEXT("WarningOKButton is not bound in FlightAttendantWarning widget."));
		return;
	}

	WarningOKButton->OnClicked.AddDynamic(this, &UFlightAttendantWarning::OnOKButtonClicked);
}

void UFlightAttendantWarning::OnOKButtonClicked()
{
	OnWarningConfirmed.Broadcast();
}
