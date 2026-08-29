// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Characters/BaseCharacter.h"
//#include "Both/CharacterAnimation.h"
#include "Components/CapsuleComponent.h"
//#include "AIController.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

/*
// Fill out your copyright notice in the Description page of Project Settings.


#include "Perception/AIPerceptionComponent.h"
#include "../END2602.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false; //if make this false, don't need below
	PrimaryActorTick.bStartWithTickEnabled = false;

	GetMesh()->SetRelativeRotation(FRotator(0.0, -90.0, 0.0));
	GetMesh()->SetRelativeLocation(FVector(0.0, 0.0, -90.0));

	//Set up weapon as a child actor component of the character
	WeaponComponent = CreateDefaultSubobject<UChildActorComponent>(TEXT("WeaponComponent"));

	if (GetMesh()->DoesSocketExist(TEXT("WeaponSocket")))
	{
		UE_LOG(LogTemp, Warning, TEXT("Socket exists"));
	}

	//Set-Up Weapon Attachment
	//Attach the weapon component to the character's mesh
	WeaponComponent->SetupAttachment(GetMesh(), TEXT("WeaponSocket"));

	//Set up health component as a child actor component of the character
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));


}

bool ABaseCharacter::CanPickupHealth(AActor* OtherActor)
{
	return false;
}

bool ABaseCharacter::CanPickupAmmo(AActor* OtherActor)
{
	return false;
}

void ABaseCharacter::SetGenericTeamId(const FGenericTeamId& NewTeamID)
{
	TeamId = NewTeamID;

	if (AAIController* AICon =
		Cast<AAIController>(GetController()))
	{
		if (IGenericTeamAgentInterface* TeamAgent =
			Cast<IGenericTeamAgentInterface>(AICon))
		{
			TeamAgent->SetGenericTeamId(NewTeamID);
		}

		//Notify AI perception component that the team ID has changed,
		// so it can update its perception accordingly.
		if (AICon->GetPerceptionComponent())
		{
			AICon->GetPerceptionComponent()
				->RequestStimuliListenerUpdate();
		}
	}

}

FGenericTeamId ABaseCharacter::GetGenericTeamId() const
{
	return TeamId;
}

//Safer than checking in begin play for the controller, because the controller may not be set yet at that point.
// This is called when the controller is set
void ABaseCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	AAIController* AICon =Cast<AAIController>(NewController);


	//Set the team ID for the AI controller as well as the pawn (player/agent), as perception component may check team of controller instead of pawn.
	if (AICon)
	{
		AICon->SetGenericTeamId(TeamId);

		if (AICon->PerceptionComponent)
		{
			AICon->PerceptionComponent
				->RequestStimuliListenerUpdate();
		}
	}
}


// Called when the game starts or when spawned
//Assigns weapon class to weapon component of character,
//Make sure weapon is a rifle, and assign character as owner of weapon
//Set animation blueprint reference for character
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();


	if (WeaponClass)
	{
		UE_LOG(Game, Warning, TEXT("WeaponClass valid"));
		WeaponComponent->SetChildActorClass(WeaponClass);
	}
	else
	{
		UE_LOG(Game, Error, TEXT("WeaponClass is NULL"));
	}


	// Weapon Pointer, use pointer from header file, set weapon object before checking null pointer
	//get the weapon from the child actor component
	Weapon = Cast<ARifle>(WeaponComponent->GetChildActor());

	//if cast fails, log an error
	if (!Weapon)
	{
		Destroy();
		UE_LOG(Game, Log, TEXT("Player needs a weapon!"));
		return;
	}

	Weapon->OwningPawn = this;
	UE_LOG(Game, Warning, TEXT("Weapon created successfully"));

	Weapon->OnRifleAttack.AddDynamic(
		this, &ABaseCharacter::HandleRifleAttack);

	//GAR Week 2
	Weapon->ReloadAmmo();

	//Setting animation BP reference
	if (!GetMesh())
	{
		UE_LOG(Game, Error, TEXT("Mesh is NULL"));
		return;
	}

	AnimationBP = Cast<UCharacterAnimation>(
		GetMesh()->GetAnimInstance());

	if (AnimationBP)
	{

		Weapon->OnReloadStart.AddDynamic(
			AnimationBP, &UCharacterAnimation::ReloadAnimation);

		AnimationBP->OnReloadNow.AddDynamic(
			Weapon, &ARifle::ReloadAmmo);

		AnimationBP->OnReloadEnded.AddDynamic(
			Weapon, &ARifle::ActionStopped);


	}
	else
	{
		UE_LOG(Game, Error,
			TEXT("No animation BP set for %s"),
			*GetName());
	}



	//Make sure you have a health component
	if (HealthComponent)
	{
		UE_LOG(Game, Warning, TEXT("Health component valid"));

		HealthComponent->OnHurt.AddDynamic(this, &ABaseCharacter::HandleHit);
		HealthComponent->OnDeath.AddDynamic(this, &ABaseCharacter::HandleDeath);

	}
	else
	{
		UE_LOG(Game, Error, TEXT("Health component is NULL"));
	}


	//KEEP THIS COMMENTED OUT
	//Setting animation BP reference, set above in weapon block now
	/*
	if (GetMesh())
	{

		AnimationBP = Cast<UCharacterAnimation>(
			GetMesh()->GetAnimInstance()
		);


		if (!AnimationBP)
		{
			UE_LOG(Game, Error,
				TEXT("AnimationBP cast failed on %s"),
				*GetName());
		}
	}
	*/
	//END KEEP THIS COMMENTED OUT

/*
}

void ABaseCharacter::HandleRifleAttack()
{

	if (AnimationBP)
	{
		AnimationBP->FireAnimation();

	}
	else
	{
		UE_LOG(Game, Error,
			TEXT("No animation BP set for %s"),
			*GetName());
	}


}

void ABaseCharacter::HandleHit(float ratio)
{

	if (AnimationBP)
	{
		AnimationBP->HitAnimation(ratio);
	}
	else
	{
		UE_LOG(Game, Error,
			TEXT("No animation BP set for %s"),
			*GetName());
	}

}

void ABaseCharacter::HandleDeath(float ratio)
{

	if (AnimationBP)
	{
		AnimationBP->DeathAnimation(ratio);
	}
	else
	{
		UE_LOG(Game, Error,
			TEXT("No animation BP set for %s"),
			*GetName());
	}


	if (Weapon)
	{
		Weapon->OwnerDied();
	}


	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}




*/