// ©Justin Camden 2019, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Runtime/Core/Public/Misc/Paths.h"
#include "HorusFilePaths.generated.h"

/**
 * Container for static const file paths and file names
 */
UCLASS()
class HORUS_API UHorusFilePaths : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	/** Localized path to the Arena Database. */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = HorusFilePaths)
		static FString ArenaDatabaseDir();

	/** Localized path to the Arena Built Files. */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = HorusFilePaths)
		static FString ArenasRootDir();

	/** Suffix for compressed arena files. */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = HorusFilePaths)
		static FString ArenaFileExt();

	/** Full file path to the Spell Card Database. */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = HorusFilePaths)
		static FString SpellCardDatabaseDir();

	/** Full file path to the Spell Card Built Files. */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = HorusFilePaths)
		static FString SpellCardsRootDir();

	/** Suffix for compressed spell card files. */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = HorusFilePaths)
		static FString SpellCardFileExt();

	
};

FORCEINLINE FString UHorusFilePaths::ArenaDatabaseDir()
{
	FString RetVal = FPaths::ProjectContentDir();
	RetVal += "Horus/BuiltFiles/Arenas/ArenaDatabase.adb";
	return RetVal;
}


FORCEINLINE FString UHorusFilePaths::ArenasRootDir()
{
	FString RetVal = FPaths::ProjectContentDir();
	RetVal += "Horus/BuiltFiles/Arenas/";
	return RetVal;
}

FORCEINLINE FString UHorusFilePaths::ArenaFileExt()
{
	return ".ad";
}

FORCEINLINE FString UHorusFilePaths::SpellCardDatabaseDir()
{
	FString RetVal = FPaths::ProjectContentDir();
	RetVal += "Horus/BuiltFiles/SpellCards/SpellCardDatabase.scdb";
	return RetVal;
}


FORCEINLINE FString UHorusFilePaths::SpellCardsRootDir()
{
	FString RetVal = FPaths::ProjectContentDir();
	RetVal += "Horus/BuiltFiles/SpellCards/";
	return RetVal;
}

FORCEINLINE FString UHorusFilePaths::SpellCardFileExt()
{
	return ".scd";
}
