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
	/** Getter for the spell card database. */
	UFUNCTION(BlueprintCallable, Category = HorusCardSubsystem)
		const TMap<FName, FHorusSpellCardData>& GetSpellCardDatabase() const { return SpellCardDatabase; }

	/*
	* Loads spell cards from an inputted database file and the files which it indicates. 
	* @param RootFilePath The root folder of the spell card files, including the database.
	* @param DatabaseFileName The name of the database file
	*/

	UFUNCTION(BlueprintCallable, Category = HorusCardSubsystem)
		bool LoadSpellCardDatabase(FString RootFilePath, const FString& DatabaseFileName);

	
private:

	/** Database of all loaded spell cards in the game. */
	UPROPERTY()
		TMap<FName, FHorusSpellCardData> SpellCardDatabase;
};
