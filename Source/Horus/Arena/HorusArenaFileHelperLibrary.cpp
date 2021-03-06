// ©Justin Camden 2019, all rights reserved.


#include "HorusArenaFileHelperLibrary.h"
#include "Runtime/Core/Public/Serialization/MemoryReader.h"
#include "Runtime/Core/Public/Serialization/ArchiveLoadCompressedProxy.h"
#include "Runtime/Core/Public/Serialization/BufferArchive.h"
#include "Runtime/Core/Public/Misc/FileHelper.h"
#include "Runtime/Core/Public/Serialization/ArchiveSaveCompressedProxy.h"
#include "Core/HorusMemoryReader.h"

// Log categories
DEFINE_LOG_CATEGORY_STATIC(LogHorusArenaFileHelperLibrary, All, All);

bool UHorusArenaFileHelperLibrary::LoadCompressedBinaryFileToArenaMap(const FString& FilePath, TMap<FName, FHorusArenaData>& OutMap, bool bAppendToMap)
{
	if (!bAppendToMap)
	{
		OutMap.Empty();
	}

	// Load file
	TArray<uint8> CompressedBytes;
	if (!FFileHelper::LoadFileToArray(CompressedBytes, *FilePath))
	{
		UE_LOG(LogHorusArenaFileHelperLibrary, Error, TEXT("ERROR: Unable to load file %s!"), *FilePath);
		return false;
	}

	// Decompress bytes
	FArchiveLoadCompressedProxy Decompressor = FArchiveLoadCompressedProxy(CompressedBytes, NAME_Zlib, ECompressionFlags::COMPRESS_NoFlags);
	if (Decompressor.GetError())
	{
		UE_LOG(LogHorusArenaFileHelperLibrary, Error, TEXT("ERROR: Unable to decompress %s!"), *FilePath);
		return false;
	}
	FBufferArchive DecompressedBytes;
	Decompressor << DecompressedBytes;

	// Read the data into an array
	TArray<FHorusArenaDataSerializable> SerializedArenas;
	FHorusMemoryReader FromBinary = FHorusMemoryReader(DecompressedBytes, true);
	FromBinary.Seek(0);
	FromBinary << SerializedArenas;

	// Copy the data into a map
	OutMap.Reserve(SerializedArenas.Num());
	for (FHorusArenaDataSerializable& CurrSerializedArena : SerializedArenas)
	{
		FHorusArenaData CurrArena;
		CurrArena.DefaultZoneClass = CurrSerializedArena.DefaultZoneClass;
		CurrArena.Rows = CurrSerializedArena.Rows;
		OutMap.Add(FName(*CurrSerializedArena.Name), CurrArena);
	}

	// Clear cache and close buffers
	FromBinary.FlushCache();
	FromBinary.Close();
	Decompressor.FlushCache();
	DecompressedBytes.Empty();
	DecompressedBytes.Close();

	return true;
}
