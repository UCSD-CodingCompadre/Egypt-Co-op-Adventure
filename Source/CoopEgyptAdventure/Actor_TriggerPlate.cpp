// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor_TriggerPlate.h"

// Sets default values
AActor_TriggerPlate::AActor_TriggerPlate()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Turn on actor replicate
	bReplicates = true;

	// Enable replicate movement
	SetReplicateMovement(true);

	// Set default activate bool
	Activated = false;

	// Create the USceneComponent for the root component
	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));

	// Set the root component
	SetRootComponent(RootComp);

	// Create the UStaticMeshComponent for the trigger mesh
	TriggerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TriggerMesh"));

	// Attach the trigger mesh to the root component
	TriggerMesh->SetupAttachment(RootComp);

	// Set the trigger mesh to replicate
	TriggerMesh->SetIsReplicated(true);

	// Hold the trigger mesh asset
	auto TriggerMeshAsset = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("/Game/StarterContent/Shapes/Shape_Cylinder.Shape_Cylinder"));

	// Check if there is a trigger mesh asset
	if (TriggerMeshAsset.Succeeded())
	{

		// Set the static mesh of the trigger mesh
		TriggerMesh->SetStaticMesh(TriggerMeshAsset.Object);

		// Set the static mesh scale
		TriggerMesh->SetRelativeScale3D(FVector(3.3f, 3.3f, 0.2f));

		// Set the static mesh location
		TriggerMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 10.0f));
	}

	// Create the UStaticMeshComponent for the mesh
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	// Attach the mesh to the root component
	Mesh->SetupAttachment(RootComp);

	// Set the mesh to replicate
	Mesh->SetIsReplicated(true);

	// Hold the mesh asset
	auto MeshAsset = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("/Game/Stylized_Egypt/Meshes/building/SM_building_part_08.SM_building_part_08"));
	
	// Check if there is a mesh asset
	if (MeshAsset.Succeeded())
	{

		// Set the static mesh of the mesh
		Mesh->SetStaticMesh(MeshAsset.Object);

		// Set the mesh scale
		Mesh->SetRelativeScale3D(FVector(4.0f, 4.0f, 0.5f));

		// Set the mesh location
		Mesh->SetRelativeLocation(FVector(0.0f, 0.0f, 7.2f));
	}

	// Create the UComponent_Transporter actor component
	Transporter = CreateDefaultSubobject<UComponent_Transporter>(TEXT("Transporter"));

	// Set the move time of the transporter component
	Transporter->MoveTime = 0.25f;

	// Set that the owner is a trigger actor
	Transporter->OwnerIsTriggerActor = true;

}

// Called when the game starts or when spawned
void AActor_TriggerPlate::BeginPlay()
{

	// Call the parent begin play function
	Super::BeginPlay();
	
	// Set the trigger mesh to not be visible
	TriggerMesh->SetVisibility(false);

	// Set the trigger mesh collision profile
	TriggerMesh->SetCollisionProfileName(FName("OverlapAll"));

	// Hold the first point for the transporter
	FVector Point1 = GetActorLocation();

	// Hold the second point for the transporter
	FVector Point2 = Point1 + FVector(0.0f, 0.0f, -10.0f);
	
	// Set the points for the transporter
	Transporter->SetPoints(Point1, Point2);
}

// Called every frame
void AActor_TriggerPlate::Tick(float DeltaTime)
{

	// Call the parent tick function
	Super::Tick(DeltaTime);
	
	// Check if the server is calling the function
	if (HasAuthority())
	{

		// Hold the overlapping actors
		TArray<AActor*> OverlappingActors;

		// Hold the trigger actor
		AActor* TriggerActor = 0;

		// Retrieve the overlapping actors
		TriggerMesh->GetOverlappingActors(OverlappingActors);

		// Loop through the overlapping actors
		for (int ActorIdx = 0; ActorIdx < OverlappingActors.Num(); ++ActorIdx)
		{

			// Hold the actor
			AActor* A = OverlappingActors[ActorIdx];

			// Check if the actor has the correct tag
			if (A->ActorHasTag("TriggerActor"))
			{

				// Set the trigger actor
				TriggerActor = A;

				// Break
				break;
			}
		}

		// Check if there is a trigger actor
		if (TriggerActor)
		{

			// Check if the trigger plate is not activated
			if (!Activated)
			{

				// Set the trigger plate to the activated state
				Activated = true;

				// Broadcast that the trigger plate was activated
				OnActivated.Broadcast();
			}
		}

		// Else there is no trigger actor
		else
		{

			//  Check if the trigger plate was activated
			if (Activated)
			{

				// Set the trigger plate to the deactivate state
				Activated = false;

				// Broadcast that the trigger plate was deactivated
				OnDeactivated.Broadcast();
			}
		}
	}
}

