// ©Justin Camden 2019, all rights reserved.


#include "HorusSyncToServerGameState.h"
#include "Runtime/Core/Public/Misc/Paths.h"
#include "Runtime/Core/Public/Misc/FileHelper.h"
#include "Player/HorusSyncToServerController.h"
#include "Runtime/Engine/Public/EngineUtils.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Runtime/Core/Public/Misc/SecureHash.h"


// Log categories
DEFINE_LOG_CATEGORY_STATIC(LogHorusSyncToServerGameState, All, All);

AHorusSyncToServerGameState::AHorusSyncToServerGameState(const FObjectInitializer& ObjectInitializer /*= FObjectInitializer::Get()*/)
	:Super(ObjectInitializer)
{
	// Initialize variables
	CurrentSyncPhase = EHorusSyncPhase::SpellCardDatabaseFileList;
	CurrentSyncPhaseState = EHorusSyncState::NotSynced;
}

void AHorusSyncToServerGameState::ServerReceiveFileValidationFailed_Implementation(const FString& FileName, const FString& ClientHashHex, AHorusSyncToServerController* NotifyingController)
{
	UE_LOG(LogHorusSyncToServerGameState, Warning, TEXT("File sync requested for file %s."), *FileName);
	FHorusFileAndHashHex& FileToSave = CachedFilesAndHashHexes[FileName];
	NotifyingController->ClientSaveAndValidateFile(FileName, FileToSave.FileLines, FileToSave.HashHex, this);
}


bool AHorusSyncToServerGameState::ServerReceiveFileValidationFailed_Validate(const FString& FileName, const FString& ClientHashHex, AHorusSyncToServerController* NotifyingController)
{
	return true;
}


void AHorusSyncToServerGameState::ServerReceiveFileValidatedSuccessfully_Implementation(const FString& FileName, AHorusSyncToServerController* NotifyingController)
{
	ControllersToUnsyncedFiles[NotifyingController].Remove(FileName);
	CheckControllerSyncState();
	return;
}


bool AHorusSyncToServerGameState::ServerReceiveFileValidatedSuccessfully_Validate(const FString& FileName, AHorusSyncToServerController* NotifyingController)
{
	return true;
}


const bool AHorusSyncToServerGameState::LoadFileList(const FString& FileName)
{
	// Load file
	FHorusFileAndHashHex LoadedFile;
	if (!FFileHelper::LoadFileToStringArray(LoadedFile.FileLines, *FileName))
	{
		UE_LOG(LogHorusSyncToServerGameState, Error, TEXT("ERROR: Unable to filelist from file %s!"), *FileName);
		return false;
	}

	// Validate files in file list
	for (FString& CurrFile : LoadedFile.FileLines)
	{
		if (!FPaths::FileExists(CurrFile))
		{
			UE_LOG(LogHorusSyncToServerGameState, Error, TEXT("ERROR: Unable to validate file %s within filelist %s!"), *CurrFile, *FileName);
			return false;
		}
	}

	// Hash the file
	FMD5Hash FileHash = FMD5Hash::HashFile(*FileName);
	if (FileHash.IsValid())
	{
		UE_LOG(LogHorusSyncToServerGameState, Error, TEXT("ERROR: Unable to hash spell card filelist %s!"), *FileName);
		return false;
	}

	// Get the hash hex and return
	LoadedFile.HashHex = BytesToHex(FileHash.GetBytes(), FileHash.GetSize());
	CachedFilesAndHashHexes.Add(FileName, LoadedFile);
	return true;
}

bool AHorusSyncToServerGameState::ProgressFileSync()
{
	switch (CurrentSyncPhase)
	{
	case EHorusSyncPhase::SpellCardDatabaseFileList:
	{
		// Load the file
		FString SpellCardDatabase = FPaths::ProjectContentDir();
		SpellCardDatabase += SpellCardDatabaseFileListName;

		// If the file loaded successfully
		if (LoadFileList(SpellCardDatabase))
		{
			// Start the sync process with each of the player controllers
			SpellCardDatabase = SpellCardDatabaseFileListName;
			CurrentSyncPhaseState = EHorusSyncState::Syncing;

			// Add the file list to each controller's files to sync
			AddUnsyncedFile(SpellCardDatabase);

			// Instruct each controller to check their version of the file against the hash hex key
			SyncClientFile(SpellCardDatabase);

			return true;
		}
		else
		{
			return false;
		}
		break;
	}
	case EHorusSyncPhase::SpellCardDatabase:
	{

	}
	default:
	{
		break;
	}
	}

	return true;
}

void AHorusSyncToServerGameState::AddUnsyncedFile(FString& FileName)
{
	// Add the file to each controller's files to sync
	for (auto& CurrController : ControllersToUnsyncedFiles)
	{
		if (CurrController.Key)
		{
			CurrController.Value.Add(FileName);
		}
		else
		{
			ControllersToUnsyncedFiles.Remove(CurrController.Key);
		}
	}
}

void AHorusSyncToServerGameState::AddUnsyncedFiles(TArray<FString>& FileNames)
{
	// Add the files to each controller's files to sync
	for (auto& CurrController : ControllersToUnsyncedFiles)
	{
		if (CurrController.Key)
		{
			for (FString& CurrFileName : FileNames)
			{
				CurrController.Value.Add(CurrFileName);
			}
		}
		else
		{
			ControllersToUnsyncedFiles.Remove(CurrController.Key);
		}
	}
}

void AHorusSyncToServerGameState::SyncClientFile(FString& FileName)
{
	// Instruct each controller to check their version of the file against the hash hex key
	FString& HashHex = CachedFilesAndHashHexes[FileName].HashHex;
	for (auto& CurrController : ControllersToUnsyncedFiles)
	{
		CurrController.Key->ClientCheckFileAgainstHash(FileName, HashHex, this);
	}

	return;
}

void AHorusSyncToServerGameState::SyncClientFiles(TArray<FString>& FileNames)
{
	// Instruct each controller to check their version of the file against the hash hex key
	for (auto& CurrController : ControllersToUnsyncedFiles)
	{
		for (FString& CurrFileName : FileNames)
		{
			CurrController.Key->ClientCheckFileAgainstHash(CurrFileName, CachedFilesAndHashHexes[CurrFileName].HashHex, this);
		}
	}

	return;
}

void AHorusSyncToServerGameState::CheckControllerSyncState()
{
	bool FilesRemainingToSync = false;

	// Check all controllers
	for (auto& CurrController : ControllersToUnsyncedFiles)
	{
		// Remove invalid controllers (such as from a disconnected player)
		if (IsValid(CurrController.Key))
		{
			// If the files remaining to sync is still larger than 0, we haven't finished syncing
			if (CurrController.Value.Num() > 0)
			{
				FilesRemainingToSync = true;
			}
		}
		else
		{
			ControllersToUnsyncedFiles.Remove(CurrController.Key);
		}
	}

	// If there are no files remaining to sync, this sync phase is complete
	if (!FilesRemainingToSync)
	{
		OnSyncPhaseComplete();
	}
}

void AHorusSyncToServerGameState::OnSyncPhaseComplete()
{
	switch (CurrentSyncPhase)
	{
	case EHorusSyncPhase::SpellCardDatabaseFileList:
	{
		CurrentSyncPhase = SpellCardDatabase;
		CurrentSyncPhaseState = EHorusSyncState::NotSynced;
		ProgressFileSync();
		break;
	}
	default:
	{
		break;
	}
	}

	return;
}

void AHorusSyncToServerGameState::PreServerSync()
{
	// Populate map of controllers
	for (TActorIterator<AHorusSyncToServerController> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		AHorusSyncToServerController* Controller = *ActorItr;
		if (Controller)
		{
			ControllersToUnsyncedFiles.Add(Controller);
		}
	}
}
