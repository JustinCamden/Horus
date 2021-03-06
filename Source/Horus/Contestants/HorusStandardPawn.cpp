// ©Justin Camden 2019, all rights reserved.


#include "HorusStandardPawn.h"
#include "Player/HorusStandardPlayerController.h"
#include "AI/HorusStandardAIController.h"

AHorusStandardPawn::AHorusStandardPawn(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	

}

void AHorusStandardPawn::BeginResolvingActions()
{
	checkf(Controller, TEXT("Error in attempting to resolve actions for %s: Controller is null!"), *GetNameSafe(this));
	if (bIsPlayer)
	{
		OwningPlayerController->Client_BeginResolvingPlayerActions();
	}
	else
	{
		OwningAIController->BeginResolvingAIActions();
	}

	return;
}

void AHorusStandardPawn::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	OwningPlayerController = Cast<AHorusStandardPlayerController>(NewController);
	if (NewController)
	{
		bIsPlayer = true;
	}
	else
	{
		OwningAIController = Cast<AHorusStandardAIController>(NewController);
		checkf(OwningAIController, TEXT("ERROR as %s attempted to possess %s: Controller is not a Horus Controller!"), *GetNameSafe(NewController), *GetNameSafe(this));
		bIsPlayer = false;
	}

	return;
}

bool AHorusStandardPawn::CanAct_Implementation(EHorusTurnPhase TurnPhase, bool bIsOwnTurn)
{
	return (TurnPhase == EHorusTurnPhase::Action && bIsOwnTurn);
}
