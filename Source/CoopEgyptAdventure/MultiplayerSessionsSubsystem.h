// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"
#include "MultiplayerSessionsSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FServerCreateDelegate, bool, WasSuccessful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FServerJoinDelegate, bool, WasSuccessful);

/**
 * 
 */
UCLASS()
class COOPEGYPTADVENTURE_API UMultiplayerSessionsSubsystem : public UGameInstanceSubsystem
{

	GENERATED_BODY()

public:

	// Constructor
	UMultiplayerSessionsSubsystem();

	/*
	Handle the initialization of the game instance subsystem
	@param FSubsystemCollectionBase& Collection the collection of subsystems in the project
	@return void
	*/
	void Initialize(FSubsystemCollectionBase& Collection) override;

	/*
	Handle the deinitialization of the game isntance subsystem
	@param none
	@return void
	*/
	void Deinitialize() override;

	// Hold the session for the subsystem
	IOnlineSessionPtr SessionInterface;

	/*
	Handle the creation of a server for the game
	@param FString ServerName the name of the server to create
	@return void
	*/
	UFUNCTION(BlueprintCallable)
	void CreateServer(FString ServerName);

	/*
	Handle joining a server for the game
	@param FString ServerName the name of the server to join
	@return void
	*/
	UFUNCTION(BlueprintCallable)
	void FindServer(FString ServerName);

	/*
	Delegate function that handles the logic after creating a server
	@param FName SessionName the name of the session
	bool WasSuccessful true if the session was created
	@return void
	*/
	void OnCreateSessionComplete(FName SessionName, bool WasSuccessful);

	/*
	Delegate function that handles the logic after destroying a server
	@param FName SessionName the name of the session
	bool WasSuccessful true if the session was destroyed
	@return void
	*/
	void OnDestroySessionComplete(FName SessionName, bool WasSuccessful);

	/*
	Delegate function that handles the logic of finding a session
	@param bool WasSuccessful true if the session was found
	@return void
	*/
	void OnFindSessionsComplete(bool WasSuccessful);

	/*
	Delegate function that handles the logic after joining a session
	@param FName SessionName the name of the session
	EOnJoinSessionCompleteResullt::Type Result the result of attempiting to
	join the session 
	@return void
	*/
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	// Hold whether the server needs to be created again after it was destroyed
	bool CreateServerAfterDestroy;

	// Hold the name of the server to destroy
	FString DestroyServerName;

	// Hold the name of the server to find
	FString ServerNameToFind;

	// Hold the name of the session
	FName MySessionName;

	// Hold the pointer to session search 
	TSharedPtr<FOnlineSessionSearch> SessionSearch;

	// Hold the delegate function when the server is created
	UPROPERTY(BlueprintAssignable)
	FServerCreateDelegate ServerCreateDel;

	// Hold the delegate function when the server is joined
	UPROPERTY(BlueprintAssignable)
	FServerJoinDelegate ServerJoinDel;
	
};
