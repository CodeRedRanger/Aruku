// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"
//create these later
//#include "Actors/CarriedItem.h"
//#include "Code/PickupInterface.h"
//#include "GenericTeamAgentInterface.h"

class UCharacterAnimation;

UCLASS(ABSTRACT)
class DOCONTHEPLANE_API ABaseCharacter : public ACharacter //, public IPickupInterface, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	//Animation Blueprint reference
	//UPROPERTY(BlueprintReadOnly)   //(EditDefaultsOnly)
		//UCharacterAnimation* AnimationBP;

	//Child component/carried items
	//UPROPERTY(VisibleAnywhere)
	//UChildActorComponent* CarryComponent;


	//UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	//TSubclassOf<AActor> ItemClass;

	//made protected because only a character would use this
	//UFUNCTION()
	//void HandleItemUse();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	//Create item
	//UPROPERTY()
	//ACarriedItem* Item;

};

/*
//AI stuff

public:
//----------------------------------------------------------------------//
// IPickupInterface
//----------------------------------------------------------------------//
	virtual bool CanPickupHealth(AActor* OtherActor) override;
	virtual bool CanPickupAmmo(AActor* OtherActor) override;


//----------------------------------------------------------------------//
// IGenericTeamAgentInterface
//----------------------------------------------------------------------//
	virtual void SetGenericTeamId(const FGenericTeamId& NewTeamID) override;

	virtual FGenericTeamId GetGenericTeamId() const override;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	FGenericTeamId TeamId;

protected:

	virtual void PossessedBy(AController* NewController) override;




*/