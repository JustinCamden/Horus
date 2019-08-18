// ©Justin Camden 2019, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "HorusSpellCard.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "HorusCardSubsystem.generated.h"


/**
 * Subsystem for managing card data, including loading, unloading, retrieving, and updating data about cards.
 */
UCLASS()
class HORUS_API UHorusCardSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	/** Function for loading a properly formatted CSV table to a map of names to SpellCardData structs. */
	UFUNCTION(BlueprintCallable, Category = HorusCardSubsystem)
		const bool LoadSpellCardMapFromCSV(TMap<FName, FHorusSpellCardData>& OutMap, const FString& Filename);

	/** Loads the inputted strings as spell cards and stores them in the database. */
	UFUNCTION(BlueprintCallable, Category = HorusCardSubsystem)
		const bool LoadCSVFilesIntoSpellCardDataBase(TArray<FString>& FileNames);

	/** Getter for the spell card database. */
	UFUNCTION(BlueprintCallable, Category = HorusCardSubsystem)
		const TMap<FName, FHorusSpellCardData>& GetSpellCardDatabase() const { return SpellCardDatabase; }

private:

	/** Database of all loaded spell cards in the game. */
	UPROPERTY()
		TMap<FName, FHorusSpellCardData> SpellCardDatabase;
};
