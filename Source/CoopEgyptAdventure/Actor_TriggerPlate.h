// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Component_Transporter.h"
#include "Actor_TriggerPlate.generated.h"

// Declare delegate function for trigger plate activating
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTriggerPlateOnActivated);

// Declare delegate function for trigger plate deactivating
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTriggerPlateOnDeactivated);


UCLASS()
class COOPEGYPTADVENTURE_API AActor_TriggerPlate : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AActor_TriggerPlate();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Hold the root component of the trigger plate
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	USceneComponent* RootComp;

	// Hold the static mesh component for the trigger mesh
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UStaticMeshComponent* TriggerMesh;

	// Hold the static mesh component for the trigger plate mesh
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UStaticMeshComponent* Mesh;

	// Hold if the trigger plate is activated
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	bool Activated;

	// Hold the trigger plate activate delegate
	UPROPERTY(BlueprintAssignable)
	FTriggerPlateOnActivated OnActivated;

	// Hold the trigger plate deactivate delegate
	UPROPERTY(BlueprintAssignable)
	FTriggerPlateOnDeactivated OnDeactivated;

	// Hold the transporter component that connects to the trigger plate
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UComponent_Transporter* Transporter;
};
