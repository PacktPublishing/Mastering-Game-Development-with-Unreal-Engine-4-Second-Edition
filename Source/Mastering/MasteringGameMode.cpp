// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "MasteringGameMode.h"
#include "UI/MasteringHUD.h"
#include "MasteringCharacter.h"
#include "UObject/ConstructorHelpers.h"

AMasteringGameMode::AMasteringGameMode()
	: Super()
{
	// use our custom HUD class
	HUDClass = AMasteringHUD::StaticClass();
}
