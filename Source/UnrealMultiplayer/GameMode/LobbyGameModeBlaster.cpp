// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/LobbyGameModeBlaster.h"

#include "GameFramework/GameStateBase.h"

void ALobbyGameModeBlaster::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	int32 numOfPlayer=GameState.Get()->PlayerArray.Num();
	if (numOfPlayer>0)
	{
		GetWorld()->GetTimerManager().SetTimer(
		   TravelTimerHandle,
		   this,
		   &ALobbyGameModeBlaster::TravelToBlasterLevel,
		   5.0f,
		   false
	   );
	}
}

void ALobbyGameModeBlaster::TravelToBlasterLevel()
{
	UWorld* World = GetWorld();
	if (World)
	{
		World->ServerTravel(TEXT("/Game/Maps/BlasterLevel?listen"));
	}
}

