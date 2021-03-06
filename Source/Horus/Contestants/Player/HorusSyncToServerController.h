// ©Justin Camden 2019, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "HorusPlayerController.h"
#include "HorusSyncToServerController.generated.h"

class AHorusSyncToServerGameState;

/**
 * Class for managing the exchange and validation of data between the player and the server when syncing files.
 */
UCLASS()
class HORUS_API AHorusSyncToServerController : public AHorusPlayerController
{
	GENERATED_BODY()

public:
	/*
	* Checks the hash of the inputted file against the inputted hex value.
	* If false or the file does not exist, will request the file from the server via HorusSyncToServerGameState.
	*/
	UFUNCTION(Client, Reliable, WithValidation)
		void ClientCheckFileAgainstHash(const FString& FileName, const FString& ServerHashHex, AHorusSyncToServerGameState* RequestingGameState);


	/*
	* Saves series of strings to the disc. 
	* Used for syncing server and client.
	*/
	UFUNCTION(Client, Reliable, WithValidation)
		void ClientSaveAndValidateFile(const FString& FileName, const TArray<uint8>& FileBytes, const FString& ServerHashHex, AHorusSyncToServerGameState* RequestingGameState, int32 FailureCount);

private:
	/** Loaded files. Cached for easy retrieval later.*/
	TMap<FString, TArray<FString>> CachedFiles;
};
