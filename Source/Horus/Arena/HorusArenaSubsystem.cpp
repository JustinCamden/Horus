// ©Justin Camden 2019, all rights reserved.


#include "HorusArenaSubsystem.h"
#include "HorusArenaFileHelperLibrary.h"
#include "Core/HorusFileHelperLibrary.h"
#include "Core/Constants/HorusFilePaths.h"

// Log category
DEFINE_LOG_CATEGORY_STATIC(LogHorusArenaSubsystem, All, All)

bool UHorusArenaSubsystem::LoadArenaDatabase()
{
	ArenaDatabase.Empty();

	// Load the list of files to load
	TArray<FString> FilesToLoad;
	if (!UHorusFileHelperLibrary::LoadCompressedBinaryFileToStringArray(UHorusFilePaths::ArenaDatabaseDir(), FilesToLoad))
	{
		UE_LOG(LogHorusArenaSubsystem, Error, TEXT("ERROR Loading Arena Database: Load of database file failed!"));
		return false;
	}

	// Load each file into the database
	TMap<FName, FHorusArenaData> TempDatabase;
	for (FString& CurrFile : FilesToLoad)
	{
		FString FilePath = UHorusFilePaths::ArenasRootDir();
		FilePath += CurrFile;

		if (!UHorusArenaFileHelperLibrary::LoadCompressedBinaryFileToArenaMap(FilePath, TempDatabase, true))
		{
			UE_LOG(LogHorusArenaSubsystem, Error, TEXT("ERROR Loading Arena Database: Load arena data file %s failed!"), *CurrFile)
			return false;
		}
	}
	ArenaDatabase = TempDatabase;
	bArenaDatabaseLoaded = true;
	UE_LOG(LogHorusArenaSubsystem, Display, TEXT("Arena Database Loaded"))

	return true;
}
