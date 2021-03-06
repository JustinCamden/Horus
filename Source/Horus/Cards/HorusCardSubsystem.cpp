// ©Justin Camden 2019, all rights reserved.


#include "HorusCardSubsystem.h"
#include "Runtime/Core/Public/Misc/FileHelper.h"
#include "Cards/HorusCardFileHelperLibrary.h"
#include "Core/HorusFileHelperLibrary.h"

// Log category
DEFINE_LOG_CATEGORY_STATIC(LogHorusCardSubsystem, All, All);

bool UHorusCardSubsystem::LoadSpellCardDatabase(FString RootFilePath, const FString& DatabaseFileName)
{
	SpellCardDatabase.Empty();

	// Construct and correct file path
	if (RootFilePath.Len() > 0 && RootFilePath[RootFilePath.Len() - 1] != '/')
	{
		RootFilePath += "/";
	}

	// Attempt to load the database
	TArray<FString> FilesToLoad;
	FString CurrPath = RootFilePath;
	CurrPath += DatabaseFileName;
	if (!UHorusFileHelperLibrary::LoadCompressedBinaryFileToStringArray(CurrPath, FilesToLoad))
	{
		UE_LOG(LogHorusCardSubsystem, Error, TEXT("ERROR: Failed to load Spell Card Database from file path %s"), *CurrPath);
		return false;
	}
	UE_LOG(LogHorusCardSubsystem, Display, TEXT("Loaded Spell Card Database from file %s"), *CurrPath);

	// Attempt to load each file in the database
	for (FString& CurrFile : FilesToLoad)
	{
		CurrPath = RootFilePath;
		CurrPath += CurrFile;
		if (!UHorusCardFileHelperLibrary::LoadCompressedBinaryFileToSpellCardMap(CurrFile, SpellCardDatabase, true))
		{
			UE_LOG(LogHorusCardSubsystem, Error, TEXT("ERROR: Failed to load Spell Card Data from file path %s"), *CurrPath);
			return false;
		}
		UE_LOG(LogHorusCardSubsystem, Display, TEXT("Loaded Spell Card Data from file %s"), *CurrPath);
	}

	UE_LOG(LogHorusCardSubsystem, Display, TEXT("File load of Spell Card Database completed successfully."));
	return true;
}
