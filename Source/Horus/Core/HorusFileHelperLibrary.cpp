// ©Justin Camden 2019, all rights reserved.


#include "HorusFileHelperLibrary.h"
#include "Runtime/Core/Public/Misc/FileHelper.h"
#include "Runtime/Core/Public/Misc/Paths.h"
#include "Runtime/Core/Public/Misc/SecureHash.h"

FString UHorusFileHelperLibrary::ProjectDir()
{
	return FPaths::ProjectDir();
}

FString UHorusFileHelperLibrary::ProjectSavedDir()
{
	return FPaths::ProjectSavedDir();
}

FString UHorusFileHelperLibrary::ProjectConfigDir()
{
	return FPaths::ProjectConfigDir();
}

FString UHorusFileHelperLibrary::ProjectContentDir()
{
	return FPaths::ProjectContentDir();
}

FString UHorusFileHelperLibrary::ProjectUserDir()
{
	return FPaths::ProjectUserDir();
}

bool UHorusFileHelperLibrary::SaveStringToFile(const FString& String, const FString& Filename)
{
	return FFileHelper::SaveStringToFile(String, *Filename);
}

bool UHorusFileHelperLibrary::SaveStringArrayToFile(const TArray<FString>& Lines, const FString& Filename)
{
	return FFileHelper::SaveStringArrayToFile(Lines, *Filename);
}

FString UHorusFileHelperLibrary::HashFileToHexString(const FString& Filename)
{
	FMD5Hash FileHash = FMD5Hash::HashFile(*Filename);
	if (FileHash.IsValid())
	{
		return BytesToHex(FileHash.GetBytes(), FileHash.GetSize());
	}
	else
	{
		return "INVALID HASH";
	}
}
