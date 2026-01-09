// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "LobbyGameModeBlaster.generated.h"

/**
 * 
 */
UCLASS()
class UNREALMULTIPLAYER_API ALobbyGameModeBlaster : public AGameMode
{
	GENERATED_BODY()
	virtual void PostLogin(APlayerController* NewPlayer) override;
	
	FTimerHandle TravelTimerHandle;

	UFUNCTION()
	void TravelToBlasterLevel();
};
