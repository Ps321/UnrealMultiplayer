// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"

#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	if (GameState)
	{
		int32 numofPlayer=GameState.Get()->PlayerArray.Num();

		APlayerState* PlayerState=NewPlayer->PlayerState;
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1,
				15.f,
				FColor::Cyan,
				FString::Printf(TEXT("Numberof players- %d and %s has joined"),numofPlayer,*PlayerState->GetPlayerName())
			);
		}
	}
}

void ALobbyGameMode::Logout(AController* ExitPlayer)
{
	Super::Logout(ExitPlayer);
	if (GameState)
	{
		int32 numofPlayer=GameState.Get()->PlayerArray.Num();

		APlayerState* PlayerState=ExitPlayer->PlayerState;
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1,
				15.f,
				FColor::Cyan,
				FString::Printf(TEXT("Numberof players- %d and %s has left"),numofPlayer,*PlayerState->GetPlayerName())
			);
		}
	}
}
