// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiplayerSessionsSubsystem.h"
#include "OnlineSubsystem.h"

void PrintString(const FString& Str)
{

	// Check for the global engine
	if (GEngine)
	{

		// Add a on screen debug message with the passed string
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Cyan, Str);
	}
}

UMultiplayerSessionsSubsystem::UMultiplayerSessionsSubsystem()
{
	// Set server creation after destruction to false
	CreateServerAfterDestroy = false;

	// Set an empty name for the server to destroy
	DestroyServerName = "";

	// Set an empty name for the server to find
	ServerNameToFind = "";

	// Set the session name
	MySessionName = FName("Co-op Egypt Adventure");
}

void UMultiplayerSessionsSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{

	// Hold the online subsystem being used
	IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();

	// Check if there is an online subsystem
	if (OnlineSubsystem)
	{

		// Hold the name of the subsystem
		FString SubsystemName = OnlineSubsystem->GetSubsystemName().ToString();
		
		// Set the session interface using the subsystem
		SessionInterface = OnlineSubsystem->GetSessionInterface();

		// Check if the session interface is valid
		if (SessionInterface.IsValid())
		{

			// Add the delegate function to create session
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UMultiplayerSessionsSubsystem::OnCreateSessionComplete);

			// Add the delegate function to destroy session
			SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UMultiplayerSessionsSubsystem::OnDestroySessionComplete);

			// Add the delegate function to find session
			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UMultiplayerSessionsSubsystem::OnFindSessionsComplete);

			// Add the delegate function to join session
			SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UMultiplayerSessionsSubsystem::OnJoinSessionComplete);
		}
	}
}

void UMultiplayerSessionsSubsystem::Deinitialize()
{
	
}

void UMultiplayerSessionsSubsystem::CreateServer(FString ServerName)
{

	// Check if the server name is empty
	if (ServerName.IsEmpty())
	{
		
		// Broadcast the server create delegate
		ServerCreateDel.Broadcast(false);

		// Return
		return;
	}

	// Hold the online session 
	FNamedOnlineSession *ExistingSession = SessionInterface->GetNamedSession(MySessionName);

	// Check if the online session exists
	if (ExistingSession)
	{

		// Set the server creation after destruction to true
		CreateServerAfterDestroy = true;

		// Set the server name to destroy
		DestroyServerName = ServerName;

		// Destroy the session
		SessionInterface->DestroySession(MySessionName);

		// Return
		return;
	}

	// Hold the session settings
	FOnlineSessionSettings SessionSettings;

	// Set allow to join in progress to true
	SessionSettings.bAllowJoinInProgress = true;

	// Set is dedicated to false
	SessionSettings.bIsDedicated = false;

	// Set should advertise to true
	SessionSettings.bShouldAdvertise = true;

	// Set the number of max players to 2
	SessionSettings.NumPublicConnections = 2;

	// Set the use of the lobbies to true
	SessionSettings.bUseLobbiesIfAvailable = true;

	// Set the use of presence to true
	SessionSettings.bUsesPresence = true;

	// Set join bia presence to true
	SessionSettings.bAllowJoinViaPresence = true;

	// Hold if the session is LAN
	bool IsLAN = false;

	// Check if the subsystem is NULL
	if (IOnlineSubsystem::Get()->GetSubsystemName() == "NULL")
	{

		// Set is LAN to true
		IsLAN = true;
	}

	// Set the lan for the session
	SessionSettings.bIsLANMatch = IsLAN;

	// Set the server name for the session
	SessionSettings.Set(FName("SERVER_NAME"), ServerName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

	// Create the session
	SessionInterface->CreateSession(0, MySessionName, SessionSettings);
}

void UMultiplayerSessionsSubsystem::FindServer(FString ServerName)
{

	// Check if the server name is empty
	if (ServerName.IsEmpty())
	{
		
		// Broadcast the server join delegate
		ServerJoinDel.Broadcast(false);

		// Return
		return;
	}

	// Set the session search to a new a SessionSearch
	SessionSearch = MakeShareable(new FOnlineSessionSearch());

	// Hold if the session is LAN
	bool IsLAN = false;

	// Check if the subsystem is NULL
	if (IOnlineSubsystem::Get()->GetSubsystemName() == "NULL")
	{

		// Set is LAN to true
		IsLAN = true;
	}

	// Query the search results if it is LAN  or not
	SessionSearch->bIsLanQuery = IsLAN;

	// Query the search results to only contain 9999 results
	SessionSearch->MaxSearchResults = 9999;

	// Set the settings for the Query to check if the server names are equal
	SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

	// Set the server to find
	ServerNameToFind = ServerName;

	// Find the session
	SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
}

void UMultiplayerSessionsSubsystem::OnCreateSessionComplete(FName SessionName, bool WasSuccessful)
{
	
	// Broadcast the server create delegate
	ServerCreateDel.Broadcast(WasSuccessful);

	// Check if the session creation was sucessful
	if (WasSuccessful)
	{

		// Travel the player to the Main level
		GetWorld()->ServerTravel("/Game/Blueprints/Levels/Main?listen");
	}
}

void UMultiplayerSessionsSubsystem::OnDestroySessionComplete(FName SessionName, bool WasSuccessful)
{
	
	// Check if the server needs to be created again 
	if (CreateServerAfterDestroy)
	{

		// Set to false since we are creating after destroying
		CreateServerAfterDestroy = false;

		// Create the server using the server name destroyed
		CreateServer(DestroyServerName);
	}
}

void UMultiplayerSessionsSubsystem::OnFindSessionsComplete(bool WasSuccessful)
{

	// Return if the session is not found
	if (!WasSuccessful) return;

	// Return if the server name is empty
	if (ServerNameToFind.IsEmpty()) return;

	// Hold the results of the session search
	TArray<FOnlineSessionSearchResult> Results = SessionSearch->SearchResults;

	// Hold a pointer to the correct result
	FOnlineSessionSearchResult* CorrectResult = 0;

	// Check if there is any results
	if (Results.Num() > 0)
	{

		// Loop through the results
		for (FOnlineSessionSearchResult Result : Results)
		{

			// Check if the result is valid
			if (Result.IsValid())
			{

				// Hold the server name 
				FString ServerName = "No-name";

				// Get the session setting with the server name
				Result.Session.SessionSettings.Get(FName("SERVER_NAME"), ServerName);

				// Check if the server name is the one we are looking for
				if (ServerName.Equals(ServerNameToFind))
				{

					// Set the correct results
					CorrectResult = &Result;

					// Break 
					break;
				}
			}
		}

		// Check if we have a correct results
		if (CorrectResult)
		{

			// Join the session
			SessionInterface->JoinSession(0, MySessionName, *CorrectResult);
		}

		// Else we failed to find a correct result
		else
		{
			
			// Reset the server to find
			ServerNameToFind = "";

			// Broadcast the server join delegate
			ServerJoinDel.Broadcast(false);
		}
	}

	// Else we found no sessions
	else
	{
		
		// Broadcast the server join delegate
		ServerJoinDel.Broadcast(false);
	}
}

void UMultiplayerSessionsSubsystem::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{

	// Broadcast the server join delegate
	ServerJoinDel.Broadcast(Result == EOnJoinSessionCompleteResult::Success);

	// Check if the result is success
	if (Result == EOnJoinSessionCompleteResult::Success)
	{

		// Hold the address of the session joined
		FString Address = "";

		// Hold if the session was found and set the address of the session
		bool Success = SessionInterface->GetResolvedConnectString(MySessionName, Address);

		// Check if we found the session address
		if (Success)
		{
			
			// Hold the first player controller of the client instance
			APlayerController *PlayerController = GetGameInstance()->GetFirstLocalPlayerController();

			// Check if we have a player controller
			if (PlayerController)
			{

				// Client travel the player controller to the address
				PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
			}
		}
		
		// Else the address can not be found 
		else
		{

			// Print the error message
			PrintString("GetResolvedConnectString returned false!");
		}
	}

	// Else the join session failed
	else
	{

		// Print the error message
		PrintString("OnJoinSessionComplete failed");
	}
}

