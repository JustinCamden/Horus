// ©Justin Camden 2019, all rights reserved.


#include "HorusStandardPawn.h"

AHorusStandardPawn::AHorusStandardPawn(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{


}

void AHorusStandardPawn::BeginResolvingActions_Implementation()
{
	return;
}

bool AHorusStandardPawn::CanAct_Implementation(EHorusTurnPhase TurnPhase, bool bIsOwnTurn)
{
	return (TurnPhase == EHorusTurnPhase::Action && bIsOwnTurn);
}
