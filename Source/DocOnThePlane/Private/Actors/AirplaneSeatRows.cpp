// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/AirplaneSeatRows.h"

AAirplaneSeatRows::AAirplaneSeatRows() : Rows(10), SeatsPerRow(3), RowSpacing(120.0f), SeatSpacing(70.0f)
{
	PrimaryActorTick.bCanEverTick = false;


	SeatHISM = CreateDefaultSubobject<UHierarchicalInstancedStaticMeshComponent>(TEXT("SeatHISM"));
	RootComponent = SeatHISM; 

	SeatHISM->SetCanEverAffectNavigation(false);
	SeatHISM->bFillCollisionUnderneathForNavmesh = false;
}

void AAirplaneSeatRows::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (!SeatHISM || !SeatHISM->GetStaticMesh())
	{
		return;
	}

	SeatHISM->ClearInstances();
	SeatHISM->SetCanEverAffectNavigation(false);

	if (!IsValid(SeatHISM->GetStaticMesh()))
	{
		return;
	}


	for (int RowIndex = 0; RowIndex < Rows; RowIndex++)
	{
		for (int SeatIndex = 0; SeatIndex < SeatsPerRow; SeatIndex++)
		{
			const float X = RowIndex * RowSpacing;
			const float Y = SeatIndex * SeatSpacing;
			const float Z = 0.0f;

			FTransform SeatTransform;
			SeatTransform.SetLocation(FVector(X, Y, Z));
			SeatTransform.SetRotation(FRotator(0.0f, 0.0f, 0.0f).Quaternion());
			SeatTransform.SetScale3D(FVector(1.0f, 1.0f, 1.0f));

			SeatHISM->AddInstance(SeatTransform);
		}
	}
}