// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NPCStatic.generated.h"

class USceneComponent;
class USphereComponent;
class UStaticMeshComponent;
class UWidgetComponent;
class UUserWidget;
class UAudioComponent;
class USoundBase;

UCLASS()
class DOCONTHEPLANE_API ANPCStatic : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANPCStatic();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

//Components

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "NPC|Components")
    USceneComponent* SceneRootCode;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "NPC|Components")
    USphereComponent* InteractionRangeCode;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "NPC|Components")
    UStaticMeshComponent* PassengerMeshCode;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "NPC|Components")
    UWidgetComponent* OutcomeMeterWidgetCode;

    UPROPERTY(BlueprintReadOnly)
	UAudioComponent* NPCReactionAudioComponent;

    UPROPERTY(EditDefaultsOnly, Category = Audio)
    USoundBase* NPCReactionSound;


//Widget Reference

    UPROPERTY(BlueprintReadOnly, Category = "NPC|UI")
    UUserWidget* OutcomeMeterRefCode;
	
};
