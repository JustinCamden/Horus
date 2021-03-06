// ©Justin Camden 2019, all rights reserved.


#include "HorusStandardGameState.h"
#include "Contestants/HorusStandardPawn.h"

AHorusStandardGameState::AHorusStandardGameState(const FObjectInitializer& ObjectInitializer /*= FObjectInitializer::Get()*/)
	:Super(ObjectInitializer)
{

}

void AHorusStandardGameState::BeginResolvingEffectStack(TArray<FHorusEffectContext>& BeginningEffectStack)
{
	check(!bIsResolvingEffectStack && "ERROR in HorusStandardGameState: Attempted to begin resolving effect stack when a stack resolution was already in progress!");

	// Initialize  effect stack and update state
	bIsResolvingEffectStack = true;
	EffectStack = BeginningEffectStack;
	EffectStackIdx = EffectStack.Num();

	// Begin the resolution process.
	AdvanceEffectStack();

	return;
}

void AHorusStandardGameState::AddEffectToInProgressStack(FHorusEffectContext& EffectToAdd)
{
	check(!bIsResolvingEffectStack && "ERROR in HorusStandardGameState: Attempted to add an effect to an in progress resolution stack when there was no resolution in progress.");

	// Add the effect and update the current active effect
	EffectStack.Insert(EffectToAdd, EffectStackIdx);
	EffectStackIdx++;

	return;
}

void AHorusStandardGameState::OnEffectResolved()
{
	// Update the number of remaining unresolved effects
	UnResolvedEffects--;

	// If all current effects are resolved, progress the effect stack
	if (UnResolvedEffects <= 0)
	{
		AdvanceEffectStack();
	}

	return;
}

void AHorusStandardGameState::AdvanceEffectStack()
{
	// Resolve effect stack from the top (most recent) down
	EffectStackIdx--;
	while (EffectStackIdx > -1)
	{
		// If we can activate the next effect
		FHorusEffectContext& CurrContext = EffectStack[EffectStackIdx];
		if (CurrContext.BaseEffect->CanActivate(CurrContext.ContextualActors))
		{
			// Increment the unresolved effect counter and activate this effect
			UnResolvedEffects++;
			CurrContext.BaseEffect->ActivateEffect(CurrContext.ContextualActors);
			return;
		}

		// Remove the effect if it could not be activated
		EffectStack.RemoveAt(EffectStackIdx);
		EffectStackIdx--;
	}

	// End the effect stack if there are no more effects waiting to be resolved
	OnEffectStackResolved();

	return;
}

void AHorusStandardGameState::AdvanceTurnToNextPhase()
{
	// Continue to the next phase if more remain
	if (CurrentTurnPhase < EHorusTurnPhase::PostEnd)
	{
		// Update new phase
		EHorusTurnPhase LastPhase = CurrentTurnPhase;
		CurrentTurnPhase = (EHorusTurnPhase)((uint8)CurrentTurnPhase + 1);
		OnTurnPhaseChangedDelegate.Broadcast(CurrentTurnPhase, LastPhase);

		// Check and resolve any effects that could be activated by the phase change.
		OnEffectStackResolvedDelegate.AddDynamic(this, &AHorusStandardGameState::OnTurnPhaseTriggeredEffectStackResolved);
		BeginResolvingEffectStack(EffectsTriggeredByTurnPhase[CurrentTurnPhase]);
	}

	// Otherwise advance to the next turn
	else
	{
		AdvanceGameToNextTurn();
	}

	return;
}

void AHorusStandardGameState::OnTurnPhaseContestantActionsResolved(AHorusStandardPawn* Contestant)
{
	// Remove the contestant from those waiting to have their actions resolved
	ContestantsWithUnresolvedActions.Remove(Contestant);

	// If there are no more contestants with actions to resolve, advance this turn phase
	if (ContestantsWithUnresolvedActions.Num() <= 0)
	{
		OnTurnPhaseContestantActionsResolved();
	}

	return;
}

void AHorusStandardGameState::AdvanceGameToNextTurn()
{
	// Loop turns
	int32 LastTurnIdx = CurrentTurnIdx;
	if (CurrentTurnIdx < ContestantsByTurnOrder.Num() - 1)
	{
		CurrentTurnIdx++;
	}
	else
	{
		CurrentTurnIdx = 0;
	}

	// Start at the beginning of the turn
	CurrentTurnPhase = EHorusTurnPhase::PreHarvest;
	OnTurnChangedDelegate.Broadcast(CurrentTurnIdx, LastTurnIdx);

	return;
}

void AHorusStandardGameState::OnEffectStackResolved()
{
	// Reset effect stack and update the state
	EffectStack.Empty();
	OnEffectStackResolvedDelegate.Broadcast();

	return;
}

void AHorusStandardGameState::OnTurnPhaseTriggeredEffectStackResolved()
{
	// Unbind the delegate
	OnEffectStackResolvedDelegate.RemoveDynamic(this, &AHorusStandardGameState::OnTurnPhaseTriggeredEffectStackResolved);

	// Resolve any outstanding actions.
	BeginTurnPhaseContestantActions();

	return;
}

void AHorusStandardGameState::BeginTurnPhaseContestantActions()
{
	// For each contestant
	for (int32 Idx = 0; Idx < ContestantsByTurnOrder.Num(); Idx++)
	{
		// If the contestant can act, add them to the list
		AHorusStandardPawn* CurrContestant = ContestantsByTurnOrder[Idx];
		if (CurrContestant->CanAct(CurrentTurnPhase, CurrentTurnIdx == Idx))
		{
			ContestantsWithUnresolvedActions.Add(CurrContestant);
		}
	}

	// Queue action resolution on each eligible contestant
	if (ContestantsWithUnresolvedActions.Num() > 0)
	{
		for (AHorusStandardPawn* CurrContestant : ContestantsWithUnresolvedActions)
		{
			CurrContestant->BeginResolvingActions();
		}
	}

	// If there are no contestants who can act, continue this turn phase.
	else
	{
		OnTurnPhaseContestantActionsResolved();
	}

	return;
}

void AHorusStandardGameState::OnTurnPhaseContestantActionsResolved()
{
	// Advance to the next turn phase
	AdvanceTurnToNextPhase();

	return;
}
