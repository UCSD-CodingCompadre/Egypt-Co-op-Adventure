// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor_CollectableKeyHolder.h"

// Sets default values
AActor_CollectableKeyHolder::AActor_CollectableKeyHolder()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Turn on actor replicates
	bReplicates = true;

	// Enable replicate movement
	SetReplicateMovement(true);

	// Create the roote component
	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));

	// Set the root component
	SetRootComponent(RootComp);

	// Create the UStaticMeshComponent for the key mesh
	KeyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("KeyMesh"));

	// Attach the key mesh to the root component
	KeyMesh->SetupAttachment(RootComp);

	// Set the key mesh to replicate
	KeyMesh->SetIsReplicated(true);

	// Set the key mesh collision profile
	KeyMesh->SetCollisionProfileName(FName("OverlapAllDynamic"));

	// Create the UStaticMeshComponent for the mesh
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	// Attach the mesh to the root component
	Mesh->SetupAttachment(RootComp);

	// Set the mesh to replicate
	Mesh->SetIsReplicated(true);

	// Sett the mesh collision profile
	Mesh->SetCollisionProfileName(FName("BlockAllDynamic"));

	// Set the key mesh rotation speed
	KeyMeshRotationSpeed = 100.0f;
}

void AActor_CollectableKeyHolder::ActivateKeyMesh()
{

	// Set the key visibility to true
	KeyMesh->SetVisibility(true);
}


// Called when the game starts or when spawned
void AActor_CollectableKeyHolder::BeginPlay()
{

	// Call the parent begin play function
	Super::BeginPlay();
	
	// Set the key mesh visibility to false
	KeyMesh->SetVisibility(false);
}

// Called every frame
void AActor_CollectableKeyHolder::Tick(float DeltaTime)
{

	// Call the parent tick function
	Super::Tick(DeltaTime);

	// Add the rotation speed to the key mesh
	KeyMesh->AddRelativeRotation(FRotator(0.0f, KeyMeshRotationSpeed * DeltaTime, 0.0f));
}


