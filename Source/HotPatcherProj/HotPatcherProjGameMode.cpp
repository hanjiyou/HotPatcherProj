// Copyright Epic Games, Inc. All Rights Reserved.

#include "HotPatcherProjGameMode.h"
#include "HotPatcherProjCharacter.h"
#include "UObject/ConstructorHelpers.h"

AHotPatcherProjGameMode::AHotPatcherProjGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
