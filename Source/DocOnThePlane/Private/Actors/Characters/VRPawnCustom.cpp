// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Characters/VRPawnCustom.h"
#include "ChaosMeter.h"
#include "Utility/MedGameInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/WidgetComponent.h"
#include "Widgets/FlightAttendantWarning.h"
#include "../DocOnThePlane.h"

// Sets default values
AVRPawnCustom::AVRPawnCustom()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true; //can't make false because using tick to test ray trace

	/*
	//Will create widget component and attach to root
	//then in begin play, will attach it to the camera that is created in VRPawn BP
	//Later can create the camera here instead of in BP
	
	//also, starting by creating an generic widget component, but later will use UChaosMeter specifically

	ChaosMeter = CreateDefaultSubobject<UWidgetComponent>(TEXT("ChaosMeter"));	

	//But I don't want it to be the root, maybe I can put it under something else??

	ChaosMeter->SetupAttachment(RootComponent); //attach to root for now, will attach to camera in begin play
	ChaosMeter->SetDrawSize(FVector2D(1000.0f, 600.0f)); //can customize later
	ChaosMeter->SetVisibility(true); 
	ChaosMeter->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	*/




}

void AVRPawnCustom::ShowFlightAttendantWarning()
{
	if (!FlightAttendantWarningComponent)
	{
		UE_LOG(Game,Error,TEXT("ShowFlightAttendantWarning: warning component is invalid."));
		return;
	}

	// Remove this check if the warning should appear repeatedly.
	if (bFlightAttendantWarningAcknowledged)
	{
		return;
	}

	FlightAttendantWarningComponent->SetHiddenInGame(false);
	FlightAttendantWarningComponent->SetVisibility(true);
	FlightAttendantWarningComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AVRPawnCustom::HideFlightAttendantWarning()
{
	if (!FlightAttendantWarningComponent)
	{
		return;
	}

	FlightAttendantWarningComponent->SetVisibility(false);
	FlightAttendantWarningComponent->SetHiddenInGame(true);
	FlightAttendantWarningComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

// Called when the game starts or when spawned
void AVRPawnCustom::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();

	if (!World)
	{
		UE_LOG(Game, Error, TEXT("ConversationComponent: World is invalid."));
		return;
	}

	//get the game instance and reset chaos score at beginning of each level
	MedGameInstanceRefCode = Cast<UMedGameInstance>(World->GetGameInstance());

	if (!MedGameInstanceRefCode)
	{
		UE_LOG(Game, Error, TEXT("ConversationComponent: MedGameInstance not created yet or is invalid."));
	}
	else
	{
		MedGameInstanceRefCode->ResetChaosScore();
	}

	//create audio component for crowd clamor sound
	CrowdClamorAudioComponent = NewObject<UAudioComponent>(this); // , UAudioComponent::StaticClass(), TEXT("CrowdClamorAudioComponent"));
	if (CrowdClamorAudioComponent)
	{
		//CrowdClamorAudioComponent->SetupAttachment(RootComponent);
		CrowdClamorAudioComponent->RegisterComponent();
		CrowdClamorAudioComponent->SetSound(MedGameInstanceRefCode->CrowdClamorSound);
		CrowdClamorAudioComponent->bAutoActivate = false;
		CrowdClamorAudioComponent->bIsUISound = false;
	}
	else
	{
		UE_LOG(Game, Error, TEXT("Failed to create CrowdClamorAudioComponent."));
	}

	TArray<UCameraComponent*> Cameras;
	GetComponents<UCameraComponent>(Cameras);

	for (UCameraComponent* Camera : Cameras)
	{
		if (Camera && Camera->ComponentHasTag(TEXT("VRCamera")))
		{
			VRCameraReference = Camera;
			break;
		}
	}

	if (!VRCameraReference)
	{
		UE_LOG(Game, Error, TEXT("%s could not find the VRCamera - tagged component."), *GetName());
		return;
	}

	//make sure class is set in BP wrapper
	if (!FlightAttendantWarningClass)
	{
		UE_LOG(Game, Error, TEXT("FlightAttendantWarningClass is NULL"));
		return;
	}

	//create a new widget component
	FlightAttendantWarningComponent = NewObject<UWidgetComponent>(this, UWidgetComponent::StaticClass(), TEXT("FlightAttendantWarningComponent"));

	if (!FlightAttendantWarningComponent)
	{
		UE_LOG(Game, Error, TEXT("%s could not create FlightAttendantWarningComponent."), *GetName());
		return;
	}

	//add component on begin play
	AddInstanceComponent(FlightAttendantWarningComponent);

	//set the component's widget class to the blueprint class set in BP wrapper
	FlightAttendantWarningComponent->SetWidgetClass(FlightAttendantWarningClass);

	//set up the widget component's properties
	FlightAttendantWarningComponent->SetWidgetSpace(EWidgetSpace::World);
	FlightAttendantWarningComponent->SetDrawSize(FVector2D(2000.0f, 1000.0f)); 

	//Needs UI collision settings (what about enabling interaction?)
	FlightAttendantWarningComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision); 
	//FlightAttendantWarningComponent->SetCollisionProfileName(TEXT("UI")); 

	//Puts above other widgets/can change location instead
	FlightAttendantWarningComponent->SetTranslucentSortPriority(100);

	//attach to the camera
	FlightAttendantWarningComponent->SetupAttachment(
		VRCameraReference
	);

	//register component
	FlightAttendantWarningComponent->RegisterComponent();


	FlightAttendantWarningObject = Cast<UFlightAttendantWarning>(FlightAttendantWarningComponent->GetUserWidgetObject()); 
	
	if (!FlightAttendantWarningObject)
	{
		UE_LOG(Game, Error, TEXT("Failed to retrieve FlightAttendantWarningObject."));
		return; 
	}

	FlightAttendantWarningObject->OnWarningConfirmed.AddDynamic(this, &AVRPawnCustom::HandleFlightAttendantWarningOK);


	//set location, 99.8 is right in front of conversation widget, but behind pause menu 99.7
	FlightAttendantWarningComponent->SetRelativeLocation(
		FVector(99.8f, 0.0f, 0.0f)
	);

	// Yaw 180 faces the camera, as you discovered with ChaosMeter.
	FlightAttendantWarningComponent->SetRelativeRotation(
		FRotator(0.0f, 180.0f, 0.0f)
	);

	FlightAttendantWarningComponent->SetRelativeScale3D(FVector(0.06f));

	// Start hidden.
	FlightAttendantWarningComponent->SetVisibility(false);
	FlightAttendantWarningComponent->SetHiddenInGame(true);


	if (!ChaosMeterClass)
	{
		UE_LOG(Game, Error, TEXT("ChaosMeterClass is NULL"));
		return;
	}


	/*
	Create component.
	Add it to the actor.
	Assign the widget class.
	Attach it.
	Register it.
	Set the transform.
	
	*/

	//Creates component at runtime; temp fix until all widgets attached to camera created in constructor
	ChaosMeter = NewObject<UWidgetComponent>(this, UWidgetComponent::StaticClass(), TEXT("ChaosMeter"));

	if (!ChaosMeter)
	{
		UE_LOG(Game, Error, TEXT("%s could not create ChaosMeter component."), *GetName());
		return;
	}

	AddInstanceComponent(ChaosMeter);

	ChaosMeter->SetWidgetClass(ChaosMeterClass);
	ChaosMeter->SetWidgetSpace(EWidgetSpace::World);
	ChaosMeter->SetDrawSize(FVector2D(1800.0f, 600.0f)); //can customize later
	ChaosMeter->SetCollisionEnabled(ECollisionEnabled::NoCollision);


	ChaosMeter->SetupAttachment(VRCameraReference);
	ChaosMeter->RegisterComponent();


	ChaosMeterObject = Cast<UChaosMeter>(ChaosMeter->GetUserWidgetObject());

	if (!ChaosMeterObject)
	{
		UE_LOG(Game,Error,TEXT("VRPawnCustom: Failed to get UChaosMeter from WidgetComponent."));
		return;
	}


	ChaosMeter->SetRelativeLocation(FVector(100.0f, 0.0f, 20.0f));
	ChaosMeter->SetRelativeRotation(FRotator(0.0f, 180.0f, 0.0f));
	ChaosMeter->SetRelativeScale3D(FVector(0.035f, 0.035f, 0.035f));

	ChaosMeter->SetVisibility(false);


}

// Called every frame
void AVRPawnCustom::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AVRPawnCustom::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AVRPawnCustom::HandleFlightAttendantWarningOK()
{
	bFlightAttendantWarningAcknowledged = true;

	HideFlightAttendantWarning();

	UE_LOG(Game,Log,TEXT("Flight attendant warning acknowledged."));

}

void AVRPawnCustom::ResetFlightAttendantWarning()
{
	bFlightAttendantWarningAcknowledged = false;
}


