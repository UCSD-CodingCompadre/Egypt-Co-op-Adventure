// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/AudioComponent.h"
#include "Actor_CollectableKeyHolder.h"
#include "Actor_CollectableKey.generated.h"

// Declare delegate function for key being collected  
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCollectableKeyOnCollected);

UCLASS()
class COOPEGYPTADVENTURE_API AActor_CollectableKey : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AActor_CollectableKey();

	/*
	Add the varaibles that need to be replicated
	@param TArray<LifetimeProperty>& OutLifetimeProps the life time properties
	for the replicated varaibles
	*/
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Hold the rotation spped
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float RotationSpeed;

	// Hold the reference the key holder actor
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	AActor_CollectableKeyHolder* KeyHolderRef;

	// Hold if the key has been collected
	UPROPERTY(ReplicatedUsing = OnRep_IsCollected, BlueprintReadWrite, VisibleAnywhere)
	bool IsCollected;

	/*
	Handle the collectable key being collected by a palyer
	@param none
	@return void
	*/
	UFUNCTION()
	void OnRep_IsCollected();

	// Hold the root component  for the collectable key
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	USceneComponent* RootComp;

	// Hold the capsule component for the collectable key collison
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UCapsuleComponent* Capsule;

	// Hold the static mesh component for the collectable key mesh
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UStaticMeshComponent* Mesh;

	// Hold the audio component for the collectable key being collected
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UAudioComponent* CollectAudio;

	// Hold the collectable key delegate function
	FCollectableKeyOnCollected OnCollected;
};
