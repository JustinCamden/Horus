// ©Justin Camden 2019, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Core/HorusGameState.h"
#include "HorusSyncToServerGameState.generated.h"


class AHorusSyncToServerController;

enum EHorusSyncPhase
{
	SpellCardDatabaseFileList,
	SpellCardDatabase
};

enum EHorusSyncState
{
	NotSynced, 
	Syncing, 
	Synced
};

struct FHorusFileAndHashHex
{
	TArray<FString> FileLines;
	FString HashHex = "";
};


/**
 * Class for managing loading and syncing data driven gameplay elements between the server and the client.
 */
UCLASS()
class HORUS_API AHorusSyncToServerGameState : public AHorusGameState
{
	GENERATED_BODY()
	
public:
	// Default construct
	AHorusSyncToServerGameState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/** Notifies the server that a file failed to validate. */
	UFUNCTION(Server, Reliable, WithValidation)
		void ServerReceiveFileValidationFailed(const FString& FileName, const FString& ClientHashHex, AHorusSyncToServerController* NotifyingController);

	/** Notifies the server that a file was validated successfully. */
	UFUNCTION(Server, Reliable, WithValidation)
		void ServerReceiveFileValidatedSuccessfully(const FString& FileName, AHorusSyncToServerController* NotifyingController);

	/** Loads the inputted file into the cached files, with an additional check to ensure that the lines are valid filenames. */
	const bool LoadFileList(const FString& FileName);


private:
	/** Name of the spell file list. Assumes the location is in the game content directory. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = HorusSyncToServerGameState, meta = (AllowPrivateAccess = "true"))
		FString SpellCardDatabaseFileListName;

	/** The current sync phase. */
	EHorusSyncPhase CurrentSyncPhase;

	/** The current sync state of the current sync phase. */
	EHorusSyncState CurrentSyncPhaseState;

	/** Map of player controllers to number of files remaining to be synced. */
	TMap<AHorusSyncToServerController*, TSet<FString>> ControllersToUnsyncedFiles;

	/** Adds a string to each key of ControllersToUnsynced files while checking the validity of each entry. */
	void AddUnsyncedFile(FString& FileName);

	/** Adds an array of strings to each key of ControllersToUnsynced files while checking the validity of each entry. */
	void AddUnsyncedFiles(TArray<FString>& FileNames);

	/*
	* Instructs each controller to check their version of the file against the hash hex key.
	* Does not check for validity as it is assumed this will be called after AddUnSyncedFile.
	*/
	void SyncClientFile(FString& FileName);

	/*
	* Instructs each controller to check their version of the files against the hash hex key.
	* Does not check for validity as it is assumed this will be called after AddUnSyncedFiles.
	*/
	void SyncClientFiles(TArray<FString>& FileNames);

	/** Cleans up any invalid controllers from the map and called OnSyncPhaseComplete if none have any remaining files to sync. */
	void CheckControllerSyncState();

	/** Called when all player controllers have fully synced their files. */
	void OnSyncPhaseComplete();

	/** Initializes variables to prepare for server sync. */
	void PreServerSync();

	/** Called to progress the syncing functions. */
	bool ProgressFileSync();

	/** Loaded files. Cached for easy retrieval later.*/
	TMap<FString, FHorusFileAndHashHex> CachedFilesAndHashHexes;


};
