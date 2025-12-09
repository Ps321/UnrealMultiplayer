// Fill out your copyright notice in the Description page of Project Settings.


#include "Menu.h"

#include "MultiplayerSessionSubsystem.h"
#include "Components/Button.h"

void UMenu::MenuSetup(int32 MaxPlayers1, FString MatchType1,FString PathToLobby1)
{
	PathToLobby=PathToLobby1+"?listen";
	MaxPlayers=MaxPlayers1;
	MatchType=MatchType1;
	AddToViewport();
	SetVisibility(ESlateVisibility::Visible);
	bIsFocusable=true;

	UWorld* World = GetWorld();
	if (World)
	{
		APlayerController* PlayerController = World->GetFirstPlayerController();
		if (PlayerController)
		{
			FInputModeUIOnly InputMode;
			InputMode.SetWidgetToFocus(TakeWidget());
			InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			PlayerController->SetInputMode(InputMode);
			PlayerController->SetShowMouseCursor(true);
		}
	}

	UGameInstance* GameInstance=GetGameInstance();
	if (GameInstance)
	{
		MultiplayerSessionSubsystem=GameInstance->GetSubsystem<UMultiplayerSessionSubsystem>();
	}

	if (MultiplayerSessionSubsystem)
	{
		MultiplayerSessionSubsystem->MultiplayerOnCreateSessionCompleteDelegate.AddDynamic(this,&ThisClass::OnCreateSession);
		MultiplayerSessionSubsystem->MultiplayerOnFindSessionsCompleteDelegate.AddUObject(this, &ThisClass::OnFindSessions);
		MultiplayerSessionSubsystem->MultiplayerOnJoinSessionCompleteDelegate.AddUObject(this, &ThisClass::OnJoinSession);
		MultiplayerSessionSubsystem->MultiplayerOnDestroySessionCompleteDelegate.AddDynamic(this, &ThisClass::OnDestroySession);
		MultiplayerSessionSubsystem->MultiplayerOnStartSessionCompleteDelegate.AddDynamic(this, &ThisClass::OnStartSession);
	}
	
}



bool UMenu::Initialize()
{
	if (!Super::Initialize())
	{
		return false;
	}
	if (HostButton)
	{
		HostButton->OnClicked.AddDynamic(this, &UMenu::HostButtonClicked);
	}
	if (JoinButton)
	{
		JoinButton->OnClicked.AddDynamic(this, &UMenu::JoinButtonClicked);
	}
	return true;
}

void UMenu::NativeDestruct()
{
	Super::NativeDestruct();
	MenuClose();
}

void UMenu::OnCreateSession(bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1,
				15.f,
				FColor::Blue,
				FString::Printf(TEXT("Created session1"))
			);
		}
		if (bWasSuccessful)
		{
			GetWorld()->ServerTravel(FString(PathToLobby));
			MenuClose();
		}
	}
	else
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1,
				15.f,
				FColor::Red,
				FString(TEXT("Failed to create session!"))
			);
		}
	}
}

void UMenu::OnFindSessions(TArray<FOnlineSessionSearchResult> searchResult, bool bWasSuccessfull)
{
	if (!MultiplayerSessionSubsystem)return;

	for (auto Result: searchResult)
	{
		
		FString SearchValue;
		Result.Session.SessionSettings.Get(FName("GameType"), SearchValue);
		if (SearchValue == MatchType)
		{
			MultiplayerSessionSubsystem->JoinGameSession(Result);
			return;
		}
	}
}

void UMenu::OnJoinSession(EOnJoinSessionCompleteResult::Type Result)
{
	IOnlineSubsystem *Subsystem=IOnlineSubsystem::Get();

	if (Subsystem)
	{
		IOnlineSessionPtr OnlineSessionInterface = Subsystem->GetSessionInterface();
		
		FString Address;
		if (OnlineSessionInterface->GetResolvedConnectString(NAME_GameSession,Address))
		{
			GEngine->AddOnScreenDebugMessage(
				-1,
				15.f,
				FColor::Blue,
				FString::Printf(TEXT("Join session complete: %s"), *Address));
			APlayerController* PlayerController = GetGameInstance()->GetFirstLocalPlayerController();
			PlayerController->ClientTravel(Address,TRAVEL_Absolute);
		}
	}
}

void UMenu::OnDestroySession(bool bWasSuccessful)
{
}

void UMenu::OnStartSession(bool bWasSuccessful)
{
}

void UMenu::HostButtonClicked()
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, TEXT("Host Button Clicked"));
	}
	MultiplayerSessionSubsystem->CreateSession(MaxPlayers,MatchType);
	
}

void UMenu::JoinButtonClicked()
{
	if (MultiplayerSessionSubsystem)
	{
		MultiplayerSessionSubsystem->FindSessions(10000);
	}
}

void UMenu::MenuClose()
{
	RemoveFromParent();
	GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, TEXT("Menu Closed"));
		APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
		if (PlayerController)
		{
			FInputModeGameOnly InputMode;
			InputMode.SetConsumeCaptureMouseDown(false);
			PlayerController->SetInputMode(InputMode);
			PlayerController->SetShowMouseCursor(false);
			PlayerController->bEnableClickEvents = false;
			PlayerController->bEnableMouseOverEvents = false;
		}
	
}
