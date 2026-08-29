// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MedKitItem.generated.h"

class UObjectLabelWidget;

UCLASS()
class DOCONTHEPLANE_API AMedKitItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMedKitItem();

	UFUNCTION(BlueprintCallable, Category = Label)
	void SetObjectLabelText(const FText& NewText);

	UFUNCTION(BlueprintCallable, Category = Label)
	void SetObjectLabelOffset(const FVector& NewOffset);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Label)
	class UWidgetComponent* LabelWidgetComponent; 

	UPROPERTY(EditDefaultsOnly, BLueprintReadOnly, Category = Label)
	TSubclassOf<UObjectLabelWidget> LabelWidgetClass; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Label)
	FText ObjectLabelText; 
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Label)
	FVector LabelOffset;

	UPROPERTY()
	UObjectLabelWidget* LabelWidgetObject; 


public:	


};
