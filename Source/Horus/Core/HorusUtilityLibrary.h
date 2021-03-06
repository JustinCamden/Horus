// ©Justin Camden 2019, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "HorusUtilityLibrary.generated.h"


template <typename T>
FORCEINLINE void GetRandomEntryFromProbabilityPackage(const TMap<T, uint8>& EntriesToRelativeProbability, T& Out)
{
	int32 ChanceRoll = 0;
	for (auto& CurrentEntry : EntriesToRelativeProbability)
	{
		ChanceRoll += CurrentEntry.Value;
	}
	ChanceRoll = FMath::RandRange(0, ChanceRoll - 1);
	int32 CurrentChancesChecked = 0;
	for (auto& CurrentEntry : EntriesToRelativeProbability)
	{
		if (CurrentEntry.Value + CurrentChancesChecked > ChanceRoll)
		{
			Out = CurrentEntry.Key;
			return;
		}
		else
		{
			CurrentChancesChecked += CurrentEntry.Value;
		}
	}


	return;
}

/**
 * Helper functions and types for misc functionality used by multiple game segments.
 */
UCLASS()
class HORUS_API UHorusUtilityLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

};
