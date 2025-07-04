// Copyright Epic Games, Inc. All Rights Reserved.

#include "ParrySlashGameMode.h"
#include "ParrySlashCharacter.h"
#include "UObject/ConstructorHelpers.h"

AParrySlashGameMode::AParrySlashGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
