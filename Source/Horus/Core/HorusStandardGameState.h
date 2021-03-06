// ©Justin Camden 2019, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Core/HorusGameState.h"
#include "Effects/HorusEffect.h"
#include "HorusStandardGameState.generated.h"

class AHorusStandardPawn;

UENUM(BlueprintType)
enum class EHorusTurnPhase : uint8
{
	PreHarvest,
	Harvest,
	PostHarvest,
	PreAction,
	Action,
	PostAction,
	PreEnd,
	End,
	PostEnd
};

USTRUCT(BlueprintType)
struct FHorusTeam
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Health = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<AHorusStandardPawn*> Contestants;
};
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FHorusEffectStackResolvedDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FHorusTurnPhaseChangeDelegate, EHorusTurnPhase, NewTurnPhase, EHorusTurnPhase, LastPhase);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FHorusTurnChangeDelegate, int32, NewTurnIndex, int32, LastTurnIndex);


/**
 * Holds information for standard format games.
 */
UCLASS()
class HORUS_API AHorusStandardGameState : public AHorusGameState
{
	GENERATED_BODY()
public:
	AHorusStandardGameState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/************************************************************************
	/* Contestants and turn order
	/************************************************************************

	/** Called whenever the game advances to the next turn */
	UPROPERTY(BlueprintAssignable, Category = HorusStandardGameState)
		FHorusTurnChangeDelegate OnTurnChangedDelegate;

	/************************************************************************
	/* Effect stack
	/************************************************************************

	/** Begins the process of resolving effects on the stack. */
	UFUNCTION(BlueprintCallable, Category = HorusStandardGameState)
		void BeginResolvingEffectStack(TArray<FHorusEffectContext>& BeginningEffectStack);

	/** Adds an effect to an effect stack in the process of being resolved. */
	UFUNCTION(BlueprintCallable, Category = HorusStandardGameState)
		void AddEffectToInProgressStack(FHorusEffectContext& EffectToAdd);

	/** Called to notify that an effect has been resolved. */
	UFUNCTION(BlueprintCallable, Category = HorusStandardGameState)
		void OnEffectResolved();

	/** Called when all triggered effects are complete. */
	UPROPERTY(BlueprintAssignable, Category = HorusStandardGameState)
		FHorusEffectStackResolvedDelegate OnEffectStackResolvedDelegate;

	/** Called whenever the turn advances to another phase. */
	UPROPERTY(BlueprintAssignable, Category = HorusStandardGameState)
		FHorusTurnPhaseChangeDelegate OnTurnPhaseChangedDelegate;


	/************************************************************************
	/* Turn phase
	/************************************************************************

	/** Call to notify that a player has resolved their actions for this phase. */
	UFUNCTION(BlueprintCallable, Category = HorusStanardGameState)
		void OnTurnPhaseContestantActionsResolved(AHorusStandardPawn* Contestant);

	/** Gets the list of effects for the inputted turn phase. */
	UFUNCTION(BlueprintCallable, Category = HorusStandardGameState)
		TArray<FHorusEffectContext>& GetTurnPhaseTriggeredEffects(EHorusTurnPhase TurnPhase) { return EffectsTriggeredByTurnPhase[TurnPhase]; }

private:
	/************************************************************************
	/* Contestants and turn order
	/************************************************************************

	/** Contains the pawns for each contestant in the game. */
	UPROPERTY(BlueprintReadOnly, Category = HorusStandardGameState, meta = (AllowPrivateAccess = "true"))
		TArray<FHorusTeam> Teams;

	/** The turn order of the contestants.*/
	UPROPERTY(BlueprintReadOnly, Category = HorusStandardGameState, meta = (AllowPrivateAccess = "true"))
		TArray<AHorusStandardPawn*> ContestantsByTurnOrder;

	/** The current turn in the turn order.*/
	UPROPERTY(BlueprintReadOnly, Category = HorusStandardGameState, meta = (AllowPrivateAccess = "true"))
		int32 CurrentTurnIdx;

	/** Call to advance the current turn to the next phase. */
	void AdvanceGameToNextTurn();


	/************************************************************************
	/* Effect stack                                                 
	/************************************************************************

	/** The stack of effects waiting to resolve. Resolves from the most recent down.*/
	UPROPERTY(BlueprintReadOnly, Category = HorusStandardGameState, meta = (AllowPrivateAccess = "true"))
		TArray<FHorusEffectContext> EffectStack;

	/** Whether an effect stack is currently being resolved. */
	bool bIsResolvingEffectStack;

	/** The index of current effect being activated in the effect stack. */
	UPROPERTY(BlueprintReadOnly, Category = HorusStandardGameState, meta = (AllowPrivateAccess = "true"))
		int32 EffectStackIdx;

	/** The number of effects waiting to be resolved. */
	UPROPERTY(BlueprintReadOnly, Category = HorusStandardGameState, meta = (AllowPrivateAccess = "true"))
		int32 UnResolvedEffects;

	/** Advances the effect stack to the next effect in order. */
	void AdvanceEffectStack();

	/** Called when all triggered effects are complete. */
	void OnEffectStackResolved();


	/************************************************************************
	/* Turn phase
	/************************************************************************

	/** The current phase of the current turn. */
	UPROPERTY(BlueprintReadOnly, Category = HorusStandardGameState, meta = (AllowPrivateAccess = "true"))
		EHorusTurnPhase CurrentTurnPhase;

	/** Effects to be triggered by phase changes. */
	TMap<EHorusTurnPhase, TArray<FHorusEffectContext>> EffectsTriggeredByTurnPhase;

	/** Called when the effect stack for the turn phase change has been resolved.*/
	void OnTurnPhaseTriggeredEffectStackResolved();

	/** Begins the process of resolving contestant actions. */
	void BeginTurnPhaseContestantActions();

	/** The contestants whose actions are waiting to be resolved. */
	TSet<AHorusStandardPawn*> ContestantsWithUnresolvedActions;

	/** Called when all contestant actions for this turn phase are resolved. */
	void OnTurnPhaseContestantActionsResolved();

	/** Advances the current turn to the next phase. */
	void AdvanceTurnToNextPhase();
};