// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Component_Transporter.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class COOPEGYPTADVENTURE_API UComponent_Transporter : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UComponent_Transporter();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Hold the start point for the transporter
	FVector StartPoint;

	// Hold the end point for the transporter
	FVector EndPoint;

	// Hold if the points are set for the transporter
	bool ArePointsSet;

	// Hold the move time of the transporter
	UPROPERTY(EditAnywhere)
	float MoveTime;

	// Hold if the owner is a trigger actor
	UPROPERTY(EditAnywhere)
	bool OwnerIsTriggerActor;

	// Hold the trigger actors for the transporter
	UPROPERTY(EditAnywhere)
	TArray<AActor*> TriggerActors;

	// Hold the amount of trigger actor activated
	UPROPERTY(VisibleAnywhere)
	int ActivatedTriggerCount;

	// Hold if all the trigger actors are activated
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	bool AllTriggerActorsTriggered;

	/*
	Set the points for the transporter
	@param FVector Point1 the start location for the transporter
	FVector Pointer the end loaction for the transporter
	@return void
	*/
	UFUNCTION(BlueprintCallable)
	void SetPoints(FVector Point1, FVector Point2);
	
	/*
	Increment trigger actor count
	@param none
	@return void
	*/
	UFUNCTION()
	void OnTriggerActorActivated();

	/*
	Decrement trigger actor count
	@param none
	@return void
	*/
	UFUNCTION()
	void OnTriggerActorDeactivated();

};
