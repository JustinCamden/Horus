// ©Justin Camden 2019, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "HorusPawn.h"
#include "Core/HorusStandardGameState.h"
#include "HorusStandardPawn.generated.h"

class UHorusElement;

/**
 * Contains the functionality for a contestant in a standard game.
 */
UCLASS()
class HORUS_API AHorusStandardPawn : public AHorusPawn
{
	GENERATED_BODY()
public:
	AHorusStandardPawn(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/** Team this contestant is associated with. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = HorusStandardPawn)
		int32 TeamID;

	/** The index of this contestant in their team. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = HorusStandardPawn)
		int32 TeamSlot;
	
	/** The element this contestant is associated with. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = HorusStandardPawn)
		TSubclassOf<UHorusElement> Element;

	/** Returns whether this contestant can act in the specified turn or phase. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = HorusStandardPawn)
		bool CanAct(EHorusTurnPhase TurnPhase, bool bIsOwnTurn);

	/** Called to begin the process of resolving this contestant's actions. */
	UFUNCTION(BlueprintNativeEvent, Category = HorusStandardPawn)
		void BeginResolvingActions();

private:

	
};
