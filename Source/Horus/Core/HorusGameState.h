// ©Justin Camden 2019, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "HorusGameState.generated.h"

/**
 * 
 */
UCLASS()
class HORUS_API AHorusGameState : public AGameState
{
	GENERATED_BODY()

public:
	AHorusGameState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
};
