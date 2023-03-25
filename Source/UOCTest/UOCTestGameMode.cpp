// Copyright Epic Games, Inc. All Rights Reserved.

#include "UOCTestGameMode.h"

#include "PlayerCamera.h"
#include "UOCTestPlayerController.h"
#include "UOCTestCharacter.h"
#include "UObject/ConstructorHelpers.h"

AUOCTestGameMode::AUOCTestGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = AUOCTestPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	// static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownCharacter"));
	// if (PlayerPawnBPClass.Class != nullptr)
	// {
	// 	DefaultPawnClass = PlayerPawnBPClass.Class;
	// }

	DefaultPawnClass = APlayerCamera::StaticClass();
	
	// set default controller to our Blueprinted controller
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownPlayerController"));
	if(PlayerControllerBPClass.Class != NULL)
	{
		PlayerControllerClass = PlayerControllerBPClass.Class;
	}
}