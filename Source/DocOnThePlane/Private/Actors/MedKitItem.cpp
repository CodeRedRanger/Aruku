// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/MedKitItem.h"
#include "Components/WidgetComponent.h"
#include "Widgets/ObjectLabelWidget.h"
#include "../DocOnThePlane.h"

// Sets default values
AMedKitItem::AMedKitItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;


	USceneComponent* SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);

	LabelWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("LabelWidgetComponent"));
	LabelWidgetComponent->SetupAttachment(SceneRoot);

	LabelWidgetComponent->SetWidgetSpace(EWidgetSpace::World); //change to screen to make flat like hud
	LabelWidgetComponent->SetDrawSize(FVector2D(500.0f, 100.0f));
	LabelWidgetComponent->SetRelativeRotation(FRotator(0.0f, 45.0f, 180.0f)); 
	LabelWidgetComponent->SetRelativeScale3D(FVector(3.0f, 0.6f, 3.0f));
	LabelWidgetComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	LabelWidgetComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 300.0f)); 
	LabelWidgetComponent->SetVisibility(false);
	LabelWidgetComponent->SetTwoSided(false);

}

void AMedKitItem::SetObjectLabelText(const FText& NewText)
{
	ObjectLabelText = NewText;

	if (LabelWidgetObject)
	{
		LabelWidgetObject->SetLabelText(NewText);
	}

}

void AMedKitItem::SetObjectLabelOffset(const FVector& NewOffset)
{
	LabelOffset = NewOffset;

	if (LabelWidgetComponent)
	{
		LabelWidgetComponent->SetRelativeLocation(NewOffset);
	}
}

// Called when the game starts or when spawned
void AMedKitItem::BeginPlay()
{
	Super::BeginPlay();

	UStaticMeshComponent* ObjectMesh = FindComponentByClass<UStaticMeshComponent>();
	
	if (!ObjectMesh)
	{
		UE_LOG(Game, Error, TEXT("%s could not find a StaticMeshComponent."), *GetName());
		return;
	}


	//More robust approach
	/*
	TArray<UStaticMeshComponent*> MeshComponents;
	GetComponents<UStaticMeshComponent>(MeshComponents);

	UStaticMeshComponent* ObjectMesh = nullptr;

	for (UStaticMeshComponent* Mesh : MeshComponents)
	{
		if (Mesh && Mesh->ComponentHasTag(TEXT("ObjectMesh")))
		{
			ObjectMesh = Mesh;
			break;
		}
	}
	
	*/

	UE_LOG(Game, Warning, TEXT("Mesh: %s  Location: %s  Scale: %s"), *ObjectMesh->GetName(), *ObjectMesh->GetComponentLocation().ToString(), *ObjectMesh->GetComponentScale().ToString());

	const FBoxSphereBounds Bounds =
		ObjectMesh->GetStaticMesh()->GetBounds();

	UE_LOG(Game, Warning, TEXT("%s Mesh Bounds Extent: %s  Radius: %f"), *GetName(), *Bounds.BoxExtent.ToString(), Bounds.SphereRadius);

	SetRootComponent(ObjectMesh);

	if (!LabelWidgetClass)
	{
		UE_LOG(Game, Warning, TEXT("LabelWidgetClass is not set in MedKitItem."));
		return;
	}


	LabelWidgetComponent->AttachToComponent(ObjectMesh,FAttachmentTransformRules::KeepRelativeTransform);
	LabelWidgetComponent->SetWidgetClass(LabelWidgetClass);
	LabelWidgetComponent->InitWidget(); 
	LabelWidgetObject = Cast<UObjectLabelWidget>(LabelWidgetComponent->GetUserWidgetObject());

	if (!LabelWidgetObject)
	{
		UE_LOG(Game, Warning, TEXT("%s could not create its label widget."), *GetName());
		return;
	}

	UE_LOG(Game, Warning, TEXT("%s: Created label widget %s"), *GetName(), *LabelWidgetObject->GetClass()->GetName());

	LabelWidgetObject->SetLabelText(ObjectLabelText);
	

	
}


