// ©Justin Camden 2019, all rights reserved.


#include "HorusStandardAIController.h"

void AHorusStandardAIController::BeginResolvingAIActions()
{
	checkf(!bIsResolvingActions, TEXT("ERROR attempting to begin resolving actions for %s: Already resolving actions!"), *GetNameSafe(this));
	bIsResolvingActions = true;
	return;
}
