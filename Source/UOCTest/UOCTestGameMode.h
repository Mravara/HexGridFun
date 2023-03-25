// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "HexGridManager.h"
#include "GameFramework/GameModeBase.h"
#include "UOCTestGameMode.generated.h"

UCLASS(minimalapi)
class AUOCTestGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AUOCTestGameMode();

	UPROPERTY()
	AHexGridManager* GridManager;
};



