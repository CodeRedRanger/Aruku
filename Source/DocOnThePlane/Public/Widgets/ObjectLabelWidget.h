// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ObjectLabelWidget.generated.h"

/**
 * 
 */
UCLASS()
class DOCONTHEPLANE_API UObjectLabelWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void SetLabelText(const FText& NewText);

protected:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* LabelText; 

};
