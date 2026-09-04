// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/GrabbableBall.h"

// Sets default values
AGrabbableBall::AGrabbableBall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	SetReplicateMovement(true); 

}

// Called when the game starts or when spawned
void AGrabbableBall::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGrabbableBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

