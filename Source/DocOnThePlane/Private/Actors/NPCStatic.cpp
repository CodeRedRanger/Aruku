// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/NPCStatic.h"
#include "Components/SceneComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "Blueprint/UserWidget.h"
#include "Components/AudioComponent.h"
#include "../DocOnThePlane.h"

// Sets default values
ANPCStatic::ANPCStatic()
{
	PrimaryActorTick.bCanEverTick = false;

	//Root
	SceneRootCode = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRootCode"));

	//Interactive Range

    InteractionRangeCode = CreateDefaultSubobject<USphereComponent>(TEXT("InteractionRangeCode"));
    InteractionRangeCode->SetupAttachment(SceneRootCode);
    InteractionRangeCode->SetSphereRadius(250.0f);

	//Passenger Mesh
	PassengerMeshCode = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PassengerMeshCode")); 
	PassengerMeshCode->SetupAttachment(SceneRootCode); 
	PassengerMeshCode->SetCollisionEnabled(ECollisionEnabled::NoCollision);


	//Outcome meter
	OutcomeMeterWidgetCode = CreateDefaultSubobject<UWidgetComponent>(TEXT("OutcomeMeterWidget")); 
	OutcomeMeterWidgetCode->SetupAttachment(SceneRootCode); 
	OutcomeMeterWidgetCode->SetupAttachment(PassengerMeshCode); 

	//create audio component for NPC reaction
	NPCReactionAudioComponent =CreateDefaultSubobject<UAudioComponent>(TEXT("NPCReactionAudioComponent"));
	NPCReactionAudioComponent->SetupAttachment(SceneRootCode);
	NPCReactionAudioComponent->bAutoActivate = false;
	NPCReactionAudioComponent->bIsUISound = false;
}


// Called when the game starts or when spawned
void ANPCStatic::BeginPlay()
{
	Super::BeginPlay();

	if (NPCReactionAudioComponent)
	{
		NPCReactionAudioComponent->SetSound(NPCReactionSound);
	}
	else
	{
		UE_LOG(Game, Error, TEXT("Failed to create NPCReactionAudioComponent."));
	}




	if (OutcomeMeterWidgetCode)
	{
		OutcomeMeterRefCode = OutcomeMeterWidgetCode->GetUserWidgetObject();
	}
	
}


/*
1. Reparent to NPCStatic
2. Verify SceneRootCode exists
3. Copy InteractionRange settings
4. Copy PassengerMesh settings
5. Copy OutcomeMeterWidget settings
6. Verify OutcomeMeterWidgetCode is attached to PassengerMeshCode
7. Delete old OutcomeMeterWidget
8. Delete old PassengerMesh
9. Delete old InteractionRange
10. Compile
11. Save
12. Test in-game
*/
