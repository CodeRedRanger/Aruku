// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FlightAttendantWarning.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFlightAttendantWarningConfirmed);

UCLASS()
class DOCONTHEPLANE_API UFlightAttendantWarning : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category = Warning)
	FOnFlightAttendantWarningConfirmed OnWarningConfirmed; 

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UButton* WarningOKButton = nullptr; 

private:
	UFUNCTION()
	void OnOKButtonClicked();

	
};
