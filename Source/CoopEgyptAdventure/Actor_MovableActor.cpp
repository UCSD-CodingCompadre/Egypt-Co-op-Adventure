// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor_MovableActor.h"

// Sets default values
AActor_MovableActor::AActor_MovableActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Turn on actor replicates
	bReplicates = true;

	// Enable replicate movement
	SetReplicateMovement(true);

	// Create the USceneComponent for the root
	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));

	// Set the root component
	SetRootComponent(RootComp);

	// Create the UArrowComponent for the start point
	Point1 = CreateDefaultSubobject<UArrowComponent>(TEXT("Point1"));

	// Attach the start point to the root component
	Point1->SetupAttachment(RootComp);

	// Set the relative location of the start point
	Point1->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));

	// Create the UArrowComponent for the end point
	Point2 = CreateDefaultSubobject<UArrowComponent>(TEXT("Point2"));

	// Attach the end point to the root component
	Point2->SetupAttachment(RootComp);

	// Set the relative location of the end point
	Point2->SetRelativeLocation(FVector(0.0f, 0.0f, 300.0f));

	// Create the UStaticMeshComponent for the mesh
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	// Attach the mesh to the root component
	Mesh->SetupAttachment(RootComp);

	// Set the mesh to replicate
	Mesh->SetIsReplicated(true);

	// Create the UComponent_Transporter for the movable actor
	Transporter = CreateDefaultSubobject<UComponent_Transporter>(TEXT("Transporter"));
}

// Called when the game starts or when spawned
void AActor_MovableActor::BeginPlay()
{

	// Call the parent begin play function
	Super::BeginPlay();
	
	// Get the global start point
	FVector StartPoint = GetActorLocation() + Point1->GetRelativeLocation();

	// Get the global end point
	FVector EndPoint = GetActorLocation() + Point2->GetRelativeLocation();

	// Set the points in the the transporter
	Transporter->SetPoints(StartPoint, EndPoint);
}

// Called every frame
void AActor_MovableActor::Tick(float DeltaTime)
{

	// Call the parent tick function
	Super::Tick(DeltaTime);
}

