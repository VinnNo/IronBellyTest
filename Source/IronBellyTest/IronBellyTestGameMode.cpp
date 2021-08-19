// Copyright Epic Games, Inc. All Rights Reserved.

#include "IronBellyTestGameMode.h"
#include "IronBellyTestHUD.h"
#include "IronBellyTestCharacter.h"
#include "UObject/ConstructorHelpers.h"

AIronBellyTestGameMode::AIronBellyTestGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AIronBellyTestHUD::StaticClass();
}
