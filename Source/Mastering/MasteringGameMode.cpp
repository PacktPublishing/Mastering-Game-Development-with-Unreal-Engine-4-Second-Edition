// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "MasteringGameMode.h"
#include "MasteringHUD.h"
#include "MasteringCharacter.h"
#include "UObject/ConstructorHelpers.h"

AMasteringGameMode::AMasteringGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AMasteringHUD::StaticClass();
}
