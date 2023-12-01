// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/ArrowComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Component_Transporter.h"
#include "Actor_MovableActor.generated.h"

UCLASS()
class COOPEGYPTADVENTURE_API AActor_MovableActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AActor_MovableActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Hold the root component for the movable actor
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	USceneComponent* RootComp;

	// Hold the arrow component for the transporter start location
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UArrowComponent* Point1;

	// Hold the arrow component for the transporter end location
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UArrowComponent* Point2;

	// Hold the static mesh component for the mesh
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UStaticMeshComponent* Mesh;

	// Hold the transporter component for the movable actor
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UComponent_Transporter* Transporter;

};
