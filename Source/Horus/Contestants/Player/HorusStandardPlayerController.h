// ©Justin Camden 2019, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Contestants/Player/HorusPlayerController.h"
#include "HorusStandardPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class HORUS_API AHorusStandardPlayerController : public AHorusPlayerController
{
	GENERATED_BODY()
public:
	/** Call to enable this player to take actions and begin the process of waiting for them to be resolved.*/
	UFUNCTION(Client, Reliable, WithValidation)
		void Client_BeginResolvingPlayerActions();
	
private:
	/** Whether this player is currently resolving their actions. */
	bool bIsResolvingActions;
};
