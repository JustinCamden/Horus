// ©Justin Camden 2019, all rights reserved.


#include "HorusFileHelperLibrary.h"
#include "Runtime/Core/Public/Misc/SecureHash.h"
#include "Runtime/Core/Public/Serialization/MemoryReader.h"
#include "Runtime/Core/Public/Serialization/ArchiveLoadCompressedProxy.h"
#include "Runtime/Core/Public/Serialization/BufferArchive.h"
#include "Runtime/Core/Public/Misc/FileHelper.h"
#include "Runtime/Core/Public/Serialization/ArchiveSaveCompressedProxy.h"
#include "Runtime/Core/Public/Misc/Paths.h"

// Log categories
DEFINE_LOG_CATEGORY_STATIC(LogHorusFileHelperLibrary, All, All);

bool UHorusFileHelperLibrary::HashFileToHexString(const FString& Filename, FString& OutHexString)
{
	FMD5Hash FileHash = FMD5Hash::HashFile(*Filename);
	if (FileHash.IsValid())
	{
		OutHexString = BytesToHex(FileHash.GetBytes(), FileHash.GetSize());
		return true;
	}
	else
	{
		OutHexString = "";
		return false;
	}
}

bool UHorusFileHelperLibrary::DecompressBytes(const TArray<uint8>& CompressedBytes, TArray<uint8>& OutDecompressedBytes)
{
	FArchiveLoadCompressedProxy Decompressor = FArchiveLoadCompressedProxy(CompressedBytes, FName(), ECompressionFlags::COMPRESS_ZLIB);
	if (Decompressor.GetError())
	{
		OutDecompressedBytes.Empty();
		return false;
	}
	FBufferArchive DecompressedBytes;
	Decompressor << DecompressedBytes;
	OutDecompressedBytes = DecompressedBytes;
	Decompressor.FlushCache();
	DecompressedBytes.Empty();
	DecompressedBytes.Close();

	return true;
}

bool UHorusFileHelperLibrary::BytesToStringArray(const TArray<uint8>& Bytes, TArray<FString>& OutStrings)
{
	if (Bytes.Num() > 0)
	{
		FMemoryReader FromBinary = FMemoryReader(Bytes, true);
		FromBinary.Seek(0);
		FromBinary << OutStrings;
		FromBinary.FlushCache();
		FromBinary.Close();
		return true;
	}

	OutStrings.Empty();
	return false;
}

bool UHorusFileHelperLibrary::CompressedBytesToStringArray(const TArray<uint8>& CompressedBytes, TArray<FString>& OutStrings)
{
	if (CompressedBytes.Num() > 0)
	{
		FArchiveLoadCompressedProxy Decompressor = FArchiveLoadCompressedProxy(CompressedBytes, NAME_Zlib, ECompressionFlags::COMPRESS_NoFlags);

		if (Decompressor.GetError())
		{
			OutStrings.Empty();
			return false;
		}

		FBufferArchive DecompressedBytes;
		Decompressor << DecompressedBytes;

		FMemoryReader FromBinary = FMemoryReader(DecompressedBytes, true);
		FromBinary.Seek(0);
		FromBinary << OutStrings;
		FromBinary.FlushCache();
		FromBinary.Close();
		Decompressor.FlushCache();
		DecompressedBytes.Empty();
		DecompressedBytes.Close();

		return true;
	}

	OutStrings.Empty();
	return false;
}

bool UHorusFileHelperLibrary::LoadCompressedBinaryFileToStringArray(const FString& FilePath, TArray<FString>& OutStrings)
{
	// Load file
	TArray<uint8> CompressedBytes;
	if (!FFileHelper::LoadFileToArray(CompressedBytes, *FilePath))
	{
		UE_LOG(LogHorusFileHelperLibrary, Error, TEXT("ERROR: Unable to load file %s!"), *FilePath);
		OutStrings.Empty();
		return false;
	}

	// Decompress bytes
	FArchiveLoadCompressedProxy Decompressor = FArchiveLoadCompressedProxy(CompressedBytes, NAME_Zlib, ECompressionFlags::COMPRESS_NoFlags);
	if (Decompressor.GetError())
	{
		UE_LOG(LogHorusFileHelperLibrary, Error, TEXT("ERROR: Unable to decompress %s!"), *FilePath);
		OutStrings.Empty();
		return false;
	}
	FBufferArchive DecompressedBytes;
	Decompressor << DecompressedBytes;

	// Read the data into an array
	FMemoryReader FromBinary = FMemoryReader(DecompressedBytes, true);
	FromBinary.Seek(0);
	FromBinary << OutStrings;

	// Clear cache and close buffers
	FromBinary.FlushCache();
	FromBinary.Close();
	Decompressor.FlushCache();
	DecompressedBytes.Empty();
	DecompressedBytes.Close();

	return true;
}

bool UHorusFileHelperLibrary::LoadCompressedBinaryFileToStringArrayAndBytes(const FString& FilePath, TArray<FString>& OutStrings, TArray<uint8>& OutCompressedBytes)
{
	// Load file
	if (!FFileHelper::LoadFileToArray(OutCompressedBytes, *FilePath))
	{
		UE_LOG(LogHorusFileHelperLibrary, Error, TEXT("ERROR: Unable to load file %s!"), *FilePath);
		OutStrings.Empty();
		OutCompressedBytes.Empty();
		return false;
	}

	// Decompress bytes
	FArchiveLoadCompressedProxy Decompressor = FArchiveLoadCompressedProxy(OutCompressedBytes, NAME_Zlib, ECompressionFlags::COMPRESS_NoFlags);
	if (Decompressor.GetError())
	{
		UE_LOG(LogHorusFileHelperLibrary, Error, TEXT("ERROR: Unable to decompress %s!"), *FilePath);
		OutStrings.Empty();
		OutCompressedBytes.Empty();
		return false;
	}
	FBufferArchive DecompressedBytes;
	Decompressor << DecompressedBytes;

	// Read the data to string arrow
	FMemoryReader FromBinary = FMemoryReader(DecompressedBytes, true);
	FromBinary.Seek(0);
	FromBinary << OutStrings;

	// Clear cache and close buffers
	FromBinary.FlushCache();
	FromBinary.Close();
	Decompressor.FlushCache();
	DecompressedBytes.Empty();
	DecompressedBytes.Close();

	return true;
}

bool UHorusFileHelperLibrary::SaveStringArrayToCompressedBinaryFile(const FString& FilePath, TArray<FString>& Strings)
{
	// Convert the strings to bytes
	FBufferArchive ToBinary;
	ToBinary << Strings;

	// Compress file
	TArray<uint8> CompressedBytes;
	FArchiveSaveCompressedProxy Compressor = FArchiveSaveCompressedProxy(CompressedBytes, NAME_Zlib, ECompressionFlags::COMPRESS_NoFlags);
	Compressor << ToBinary;
	Compressor.Flush();

	// Save the file
	if (!FFileHelper::SaveArrayToFile(CompressedBytes, *FilePath))
	{
		UE_LOG(LogHorusFileHelperLibrary, Error, TEXT("ERROR: Unable to save %s!"), *FilePath);

		// Free arrays and close buffer
		Compressor.FlushCache();
		CompressedBytes.Empty();
		ToBinary.FlushCache();
		ToBinary.Empty();
		ToBinary.Close();

		return false;
	}

	// Free arrays and close buffer
	Compressor.FlushCache();
	CompressedBytes.Empty();
	ToBinary.FlushCache();
	ToBinary.Empty();
	ToBinary.Close();

	return true;
}

void UHorusFileHelperLibrary::FindFilesInDirectory(TArray<FString>& OutFiles, FString RootFolderPath, FString ExtensionFilter, bool bSearchChildDirectories /*= true*/, bool bReturnFullPaths /*= true*/)
{
	FPaths::NormalizeDirectoryName(RootFolderPath);
	IFileManager& FileManager = IFileManager::Get();
	if (ExtensionFilter == "")
	{
		ExtensionFilter = "*.*";
	}
	else
	{
		ExtensionFilter = (ExtensionFilter.Left(1) == "." ? "*" + ExtensionFilter : "*." + ExtensionFilter);
	}

	if (bSearchChildDirectories)
	{
		FileManager.FindFilesRecursive(OutFiles, *RootFolderPath, *ExtensionFilter, true, false);
		if (!bReturnFullPaths)
		{
			for (FString& CurrFilePath : OutFiles)
			{
				int32 Idx = CurrFilePath.Len() -1;
				bool bShouldContinue = Idx >= 0;
				while (bShouldContinue)
				{
					if (CurrFilePath[Idx] == '/')
					{
						CurrFilePath = CurrFilePath.RightChop(Idx + 1);
						bShouldContinue = false;
					}
					else
					{
						Idx--;
						bShouldContinue = Idx >= 0;
					}
				}
			}
		}
	}
	else
	{
		RootFolderPath = RootFolderPath + "/";
		FString FullPath = RootFolderPath + ExtensionFilter;
		FileManager.FindFiles(OutFiles, *FullPath, true, false);
		if (bReturnFullPaths)
		{
			for (FString& CurrFilePath : OutFiles)
			{
				CurrFilePath = RootFolderPath + CurrFilePath;
			}
		}
	}

	return;
}
