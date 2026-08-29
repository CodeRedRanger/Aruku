// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "AirplaneSeatRows.generated.h"

UCLASS()
class DOCONTHEPLANE_API AAirplaneSeatRows : public AActor
{
	GENERATED_BODY()
	
public:
	AAirplaneSeatRows();

protected:
	virtual void OnConstruction(const FTransform& Transform) override;

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UHierarchicalInstancedStaticMeshComponent* SeatHISM;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Seat Layout")
	int Rows;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Seat Layout")
	int SeatsPerRow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Seat Layout")
	float RowSpacing;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Seat Layout")
	float SeatSpacing;
};