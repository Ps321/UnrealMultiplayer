// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/OverHeadWidget.h"

#include "Components/TextBlock.h"

void UOverHeadWidget::SetDisplayText(FString Text)
{
	if (DisplayText)
	{
		DisplayText->SetText(FText::FromString(Text));
	}
}

void UOverHeadWidget::ShowPlayerNetRole(APawn* Pawn)
{
	ENetRole LocalRole=Pawn->GetLocalRole();
	FString Role;
	switch (LocalRole)
	{
	case ROLE_Authority:
		Role="Authority";
		break;
	case ROLE_AutonomousProxy:
		Role="AutonomousProxy";
		break;
	case ROLE_SimulatedProxy:
		Role="SimulatedProxy";
		break;
	case ROLE_None:
		Role="None";
		break;
	}
	SetDisplayText(Role);
}

void UOverHeadWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// Register delegate
	FWorldDelegates::LevelRemovedFromWorld.AddUObject(
		this,
		&UOverHeadWidget::HandleLevelRemoved
	);
}

void UOverHeadWidget::NativeDestruct()
{
	
	FWorldDelegates::LevelRemovedFromWorld.RemoveAll(this);

	Super::NativeDestruct();
}

void UOverHeadWidget::HandleLevelRemoved(ULevel* Level, UWorld* World)
{
	RemoveFromParent();
}