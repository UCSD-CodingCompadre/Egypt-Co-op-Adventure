// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Actor_WinArea.generated.h"

// Declare delegate function for users winning
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FWinAreaOnWinCondition);

UCLASS()
class COOPEGYPTADVENTURE_API AActor_WinArea : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AActor_WinArea();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Hold if the players have won
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	bool WinCondition;

	// Hold the box component for the win area
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UBoxComponent* WinAreaBox;

	/*
	A netmultcast reliable function that broadcasts that the users won
	@param none
	@return void
	*/
	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPCWin();

	// Hold the delegate function for the users winning
	UPROPERTY(BlueprintAssignable)
	FWinAreaOnWinCondition OnWinCondition;
};
