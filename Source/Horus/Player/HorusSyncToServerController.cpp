// ©Justin Camden 2019, all rights reserved.


#include "HorusSyncToServerController.h"
#include "Runtime/Core/Public/Misc/Paths.h"
#include "Runtime/Core/Public/Misc/FileHelper.h"
#include "Runtime/Core/Public/Misc/SecureHash.h"
#include "Core/HorusSyncToServerGameState.h"

DEFINE_LOG_CATEGORY_STATIC(LogHorusSyncToServerController, All, All);

void AHorusSyncToServerController::ClientCheckFileAgainstHash_Implementation(const FString& FileName, const FString& ServerHashHex, AHorusSyncToServerGameState* RequestingGameState)
{
	checkf(RequestingGameState != nullptr, TEXT("%s: ERROR while checking file %s against hashed hex value: RequestingGameState was null!"), *GetNameSafe(this), *FileName);

	// Compare the hash value
	FMD5Hash FileHash = FMD5Hash::HashFile(*FileName);
	if (FileHash.IsValid())
	{
		// If the hex values match, message the server that success was achieved
		FString ClientHash = BytesToHex(FileHash.GetBytes(), FileHash.GetSize());
		if (ClientHash == ServerHashHex)
		{
			RequestingGameState->ServerReceiveFileValidatedSuccessfully(FileName, this);
			return;
		}
		else
		{
			UE_LOG(LogHorusSyncToServerController, Warning, TEXT("%s: File %s failed validation! Client and server hashes are not equal! Notifying server."), *GetNameSafe(this), *FileName);
			RequestingGameState->ServerReceiveFileValidationFailed(FileName, ClientHash, this);
		}
	}

	// Else, message the server that file validation failed
	UE_LOG(LogHorusSyncToServerController, Warning, TEXT("%s: File %s failed validation! Hash is invalid! Notifying server."), *GetNameSafe(this), *FileName);
	RequestingGameState->ServerReceiveFileValidationFailed(FileName, "", this);
	return;
}

bool AHorusSyncToServerController::ClientCheckFileAgainstHash_Validate(const FString& FileName, const FString& ServerHashHex, AHorusSyncToServerGameState* RequestingGameState)
{
	return true;
}

void AHorusSyncToServerController::ClientSaveAndValidateFile_Implementation(const FString& FileName, const TArray<FString>& FileLines, const FString& ServerHashHex, AHorusSyncToServerGameState* RequestingGameState)
{

}

bool AHorusSyncToServerController::ClientSaveAndValidateFile_Validate(const FString& FileName, const TArray<FString>& FileLines, const FString& ServerHashHex, AHorusSyncToServerGameState* RequestingGameState)
{
	return true;
}
