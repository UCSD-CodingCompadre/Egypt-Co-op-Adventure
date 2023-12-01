// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor_WinArea.h"
#include "CoopEgyptAdventureCharacter.h"

// Sets default values
AActor_WinArea::AActor_WinArea()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Turn on replicate
	bReplicates = true;

	// Create the UBoxComponet for the the win area
	WinAreaBox = CreateDefaultSubobject<UBoxComponent>(TEXT("WinAreaBox"));

	// Set the root component as the win area box
	SetRootComponent(WinAreaBox);

	// Set the win condition to false
	WinCondition = false;
}

void AActor_WinArea::MulticastRPCWin_Implementation()
{

	// Broadcast the win condition
	OnWinCondition.Broadcast();
}

// Called when the game starts or when spawned
void AActor_WinArea::BeginPlay()
{

	// Call the parent begin play function
	Super::BeginPlay();
}

// Called every frame
void AActor_WinArea::Tick(float DeltaTime)
{

	// Call the parent tick function
	Super::Tick(DeltaTime);

	// Check if the call is coming from the server
	if (HasAuthority())
	{

		// Check if the users have not won
		if (!WinCondition)
		{

			// Hold the overlapping actors
			TArray<AActor*> OverlapActors;

			// Set the overlapping actors
			WinAreaBox->GetOverlappingActors(OverlapActors, ACoopEgyptAdventureCharacter::StaticClass());

			// Set if the users have won
			WinCondition = (OverlapActors.Num() == 2);

			// Check if the users won
			if (WinCondition)
			{
				
				// Call the win function
				MulticastRPCWin();
			}
		}
	}
}



