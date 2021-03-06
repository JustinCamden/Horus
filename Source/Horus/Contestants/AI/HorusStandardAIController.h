// ©Justin Camden 2019, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Contestants/AI/HorusAIController.h"
#include "HorusStandardAIController.generated.h"

/**
 * 
 */
UCLASS()
class HORUS_API AHorusStandardAIController : public AHorusAIController
{
	GENERATED_BODY()

public:
	/** Signals this AI contestant to begin taking actions. */
	void BeginResolvingAIActions();

private:
	/** Whether this AI controller is currently resolving its actions. */
	bool bIsResolvingActions;
	
};
