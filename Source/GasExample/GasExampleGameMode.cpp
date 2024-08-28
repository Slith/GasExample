// Copyright Epic Games, Inc. All Rights Reserved.

#include "GasExampleGameMode.h"
#include "GasExampleCharacter.h"
#include "UObject/ConstructorHelpers.h"

AGasExampleGameMode::AGasExampleGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
