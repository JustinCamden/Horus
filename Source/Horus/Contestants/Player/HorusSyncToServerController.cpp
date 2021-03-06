// ©Justin Camden 2019, all rights reserved.


#include "HorusSyncToServerController.h"
#include "Runtime/Core/Public/Misc/Paths.h"
#include "Runtime/Core/Public/Misc/FileHelper.h"
#include "Core/HorusSyncToServerGameState.h"
#include "Core/HorusFileHelperLibrary.h"

DEFINE_LOG_CATEGORY_STATIC(LogHorusSyncToServerController, All, All);

void AHorusSyncToServerController::ClientCheckFileAgainstHash_Implementation(const FString& FileName, const FString& ServerHashHex, AHorusSyncToServerGameState* RequestingGameState)
{
	// Compare the hash value
	FString FilePath = FPaths::ProjectSavedDir();
	FilePath += FileName;
	FString ClientHash = "";
	if (UHorusFileHelperLibrary::HashFileToHexString(FilePath, ClientHash))
	{
		if (ClientHash == ServerHashHex)
		{
			RequestingGameState->ServerReceiveFileValidatedSuccessfully(FileName, this);
			return;
		}
		else
		{
			UE_LOG(LogHorusSyncToServerController, Warning, TEXT("File %s failed validation! Client and server hashes are not equal! Notifying server."), *FileName);
			RequestingGameState->ServerReceiveFileValidationFailed(FileName, ClientHash, this, 1);
		}
	}

	// Else, message the server that file validation failed
	UE_LOG(LogHorusSyncToServerController, Warning, TEXT("File %s failed validation! Hash is invalid! Notifying server."), *FileName);
	RequestingGameState->ServerReceiveFileValidationFailed(FileName, "", this, 1);
	return;
}

bool AHorusSyncToServerController::ClientCheckFileAgainstHash_Validate(const FString& FileName, const FString& ServerHashHex, AHorusSyncToServerGameState* RequestingGameState)
{
	if (!RequestingGameState)
	{
		UE_LOG(LogHorusSyncToServerController, Error, TEXT("Error receiving file check request: Gamestate was nullptr. Disconnecting from server."));
		return false;
	}
	return true;
}

void AHorusSyncToServerController::ClientSaveAndValidateFile_Implementation(const FString& FileName, const TArray<uint8>& FileBytes, const FString& ServerHashHex, AHorusSyncToServerGameState* RequestingGameState, int32 FailureCount)
{
	// Attempt to save the file
	FString FilePath = FPaths::ProjectSavedDir();
	FilePath += FileName;
	if (!FFileHelper::SaveArrayToFile(FileBytes, *FilePath))
	{
		UE_LOG(LogHorusSyncToServerController, Warning, TEXT("File save failed! Notifying server!"), *FileName);
		RequestingGameState->ServerReceiveFileValidationFailed(FileName, "", this, FailureCount + 1);
		return;
	}

	// Compare the hash value
	FString ClientHash = "";
	if (UHorusFileHelperLibrary::HashFileToHexString(FilePath, ClientHash))
	{
		if (ClientHash == ServerHashHex)
		{
			RequestingGameState->ServerReceiveFileValidatedSuccessfully(FileName, this);
			return;
		}
		else
		{
			UE_LOG(LogHorusSyncToServerController, Warning, TEXT("File %s failed validation! Client and server hashes are not equal! Notifying server."), *FileName);
			RequestingGameState->ServerReceiveFileValidationFailed(FileName, ClientHash, this, FailureCount + 1);
		}
	}

	// Else, message the server that file validation failed
	UE_LOG(LogHorusSyncToServerController, Warning, TEXT("File %s failed validation! Hash is invalid! Notifying server."), *FileName);
	RequestingGameState->ServerReceiveFileValidationFailed(FileName, "", this, FailureCount + 1);
	return;
}

bool AHorusSyncToServerController::ClientSaveAndValidateFile_Validate(const FString& FileName, const TArray<uint8>& FileBytes, const FString& ServerHashHex, AHorusSyncToServerGameState* RequestingGameState, int32 FailureCount)
{
	if (!RequestingGameState)
	{
		UE_LOG(LogHorusSyncToServerController, Error, TEXT("Error receiving file save request: Gamestate was nullptr. Disconnecting from server."));
		return false;
	}

	return true;
}
