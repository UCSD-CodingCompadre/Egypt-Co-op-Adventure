// Fill out your copyright notice in the Description page of Project Settings.


#include "Component_Transporter.h"
#include "Actor_CollectableKey.h"
#include "Actor_TriggerPlate.h"

// Sets default values for this component's properties
UComponent_Transporter::UComponent_Transporter()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


void UComponent_Transporter::SetPoints(FVector Point1, FVector Point2)
{

	// Check if the points are equal to each other and return if they are
	if (Point1.Equals(Point2)) return;

	// Set the start point
	StartPoint = Point1;

	// Set the end point
	EndPoint = Point2;
	
	// Set that the points are set
	ArePointsSet = true;
}

void UComponent_Transporter::OnTriggerActorActivated()
{

	// Increment the trigger actor count
	ActivatedTriggerCount++;
}

void UComponent_Transporter::OnTriggerActorDeactivated()
{

	// Decrement the trigger actor count
	ActivatedTriggerCount--;
}

// Called when the game starts
void UComponent_Transporter::BeginPlay()
{
	
	// Call the parent begin play function
	Super::BeginPlay();

	// Check if the owner is a trigger actor
	if (OwnerIsTriggerActor)
	{

		// Add the owning actor to the trigger actors
		TriggerActors.Add(GetOwner());
	}

	// Loop through all trigger actors
	for (AActor* TA : TriggerActors)
	{

		// Hold the trigger plate actor
		AActor_TriggerPlate *TriggerPlateActor = Cast<AActor_TriggerPlate>(TA);

		// Check if it is a trigger plate actor
		if (TriggerPlateActor)
		{

			// Add the activate function to the activate delegate
			TriggerPlateActor->OnActivated.AddDynamic(this, &UComponent_Transporter::OnTriggerActorActivated);

			// Add the deactivate function to the deactivate delegate
			TriggerPlateActor->OnDeactivated.AddDynamic(this, &UComponent_Transporter::OnTriggerActorDeactivated);

			// Loop to next actor
			continue;
		}

		// Hold the collectable key actor 
		AActor_CollectableKey* KeyActor = Cast<AActor_CollectableKey>(TA);
		
		// Check if it is a key actor
		if (KeyActor)
		{

			// Add the activate function to the deactivate delegate
			KeyActor->OnCollected.AddDynamic(this, &UComponent_Transporter::OnTriggerActorActivated);
		}
	}
}

// Called every frame
void UComponent_Transporter::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{

	// Call the parent tick component function
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Check if there is trigger actors
	if (TriggerActors.Num() > 0)
	{

		// Set if all the trigger actors are activated
		AllTriggerActorsTriggered = (ActivatedTriggerCount >= TriggerActors.Num());
	}

	// Hold the owner of the transporter
	AActor* MyOwner = GetOwner();

	// Check if there is a owner, points are set, and call is from server
	if (MyOwner && MyOwner->HasAuthority() && ArePointsSet)
	{

		// Hold the current actor location
		FVector CurrentLocation = MyOwner->GetActorLocation();

		// Hold the speed of the transporter
		float Speed = FVector::Distance(StartPoint, EndPoint) / MoveTime;

		// Hold the target location to move ot
		FVector TargetLocation = AllTriggerActorsTriggered ? EndPoint : StartPoint;

		// Check if the current location has not reached the target location
		if (!CurrentLocation.Equals(TargetLocation))
		{

			// Hold the new location to move to
			FVector NewLocation = FMath::VInterpConstantTo(CurrentLocation, TargetLocation, DeltaTime, Speed);

			// Set the new actor location
			MyOwner->SetActorLocation(NewLocation);
		}
	}
}



