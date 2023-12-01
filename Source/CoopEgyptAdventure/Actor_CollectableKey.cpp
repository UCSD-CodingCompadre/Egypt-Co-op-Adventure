// Fill out your copyright notice in the Description page of Project Settings.

#include "Actor_CollectableKey.h"
#include "Net/UnrealNetwork.h"
#include "CoopEgyptAdventureCharacter.h"

// Sets default values
AActor_CollectableKey::AActor_CollectableKey()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Turn on actor replicates
	bReplicates = true;

	// Enable replicate movement for the actor
	SetReplicateMovement(true);

	// Create the root component
	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	
	// Set the root component
	SetRootComponent(RootComp);

	// Create the UCapsuleComponent for the capsule
	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));

	// Attach the capsule to the root component
	Capsule->SetupAttachment(RootComp);

	// Set the capsule to replicate
	Capsule->SetIsReplicated(true);

	// Set the capsule collision profile
	Capsule->SetCollisionProfileName(FName("OverlapAllDynamic"));

	// Set the capsule height
	Capsule->SetCapsuleHalfHeight(150.0f);

	// Set the capsule radius
	Capsule->SetCapsuleRadius(100.0f);

	// Create the UStaticMeshComponent for the mesh
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	// Attach the mesh to the root compoent
	Mesh->SetupAttachment(RootComp);

	// Set the mesh to replicate
	Mesh->SetIsReplicated(true);

	// Set the mesh collision profile
	Mesh->SetCollisionProfileName(FName("OverlapAllDynamic"));

	// Create the UAudioComponent for the collect audio
	CollectAudio = CreateDefaultSubobject<UAudioComponent>(TEXT("CollectAudio"));

	// Attach the collect audio to the mesh
	CollectAudio->SetupAttachment(RootComp);

	// Set the collect autio to not automatically activate
	CollectAudio->SetAutoActivate(false);

	// Set the rotation speed 
	RotationSpeed = 100.0f;
}

void AActor_CollectableKey::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const 
{

	// Call the parent lifetime replicate props function
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Add the varaible that is being replicated
	DOREPLIFETIME(AActor_CollectableKey, IsCollected);
}

void AActor_CollectableKey::OnRep_IsCollected()
{

	// Check if the server is making the call
	if (HasAuthority())
	{
		
		// Check if the key has been collected
		if (IsCollected)
		{

			// Broadcast the on collect function
			OnCollected.Broadcast();
		}
	}

	// Set the visibility of the key to hidden
	Mesh->SetVisibility(!IsCollected);
	
	// Play the collect audio sound
	CollectAudio->Play();

	// Check if the key is collected
	if (IsCollected)
	{

		// Check if we have a key holder ref
		if (KeyHolderRef)
		{

			// Activate the key mesh on the key holder
			KeyHolderRef->ActivateKeyMesh();			
		}
	}
}

// Called when the game starts or when spawned
void AActor_CollectableKey::BeginPlay()
{

	// Call the parent begin play function
	Super::BeginPlay();
	
}

// Called every frame
void AActor_CollectableKey::Tick(float DeltaTime)
{

	// Call the parent tick function
	Super::Tick(DeltaTime);

	// Check if the tick is coming from the server
	if (HasAuthority())
	{

		// Rotate the key
		Mesh->AddRelativeRotation(FRotator(0.0f, RotationSpeed*DeltaTime, 0.0f));

		// Hold the overlapping actors
		TArray<AActor*> OverlapActors;

		// Set the overlapping actors from the capsule
		Capsule->GetOverlappingActors(OverlapActors, ACoopEgyptAdventureCharacter::StaticClass());

		// Check if there is overlapping actors
		if (OverlapActors.Num() > 0)
		{
			
			// Check if the has not been collected
			if (!IsCollected)
			{

				// Set the key to be collected
				IsCollected = true;

				// Call the collect function
				OnRep_IsCollected();
			}
		}
	}
}



