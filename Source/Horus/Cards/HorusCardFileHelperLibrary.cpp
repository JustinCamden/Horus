// ©Justin Camden 2019, all rights reserved.


#include "HorusCardFileHelperLibrary.h"
#include "Runtime/Core/Public/Serialization/MemoryReader.h"
#include "Runtime/Core/Public/Serialization/ArchiveLoadCompressedProxy.h"
#include "Runtime/Core/Public/Serialization/BufferArchive.h"
#include "Runtime/Core/Public/Misc/FileHelper.h"
#include "Runtime/Core/Public/Serialization/ArchiveSaveCompressedProxy.h"
#include "Core/HorusMemoryReader.h"
#include "Core/Constants/HorusFilePaths.h"

// Log categories
DEFINE_LOG_CATEGORY_STATIC(LogHorusCardFileHelperLibrary, All, All);

bool UHorusCardFileHelperLibrary::LoadCompressedBinaryFileToSpellCardMap(const FString& FilePath, TMap<FName, FHorusSpellCardData>& OutMap, bool bAppendDataToMap)
{
	if (!bAppendDataToMap)
	{
		OutMap.Empty();
	}

	// Load file
	TArray<uint8> CompressedBytes;
	if (!FFileHelper::LoadFileToArray(CompressedBytes, *FilePath))
	{
		UE_LOG(LogHorusCardFileHelperLibrary, Error, TEXT("ERROR: Unable to load file %s!"), *FilePath);
		return false;
	}

	// Decompress bytes
	FArchiveLoadCompressedProxy Decompressor = FArchiveLoadCompressedProxy(CompressedBytes, NAME_Zlib, ECompressionFlags::COMPRESS_NoFlags);
	if (Decompressor.GetError())
	{
		UE_LOG(LogHorusCardFileHelperLibrary, Error, TEXT("ERROR: Unable to decompress %s!"), *FilePath);
		return false;
	}
	FBufferArchive DecompressedBytes;
	Decompressor << DecompressedBytes;

	// Read the data into an array
	TArray<FHorusSpellCardDataSerializable> SerializedSpellCards;
	FHorusMemoryReader FromBinary = FHorusMemoryReader(DecompressedBytes, true);
	FromBinary.Seek(0);
	FromBinary << SerializedSpellCards;

	// Copy the data into a map
	OutMap.Reserve(SerializedSpellCards.Num());
	for (FHorusSpellCardDataSerializable& CurrSerializedSpellCard : SerializedSpellCards)
	{
		FHorusSpellCardData CurrSpellCard;
		CurrSpellCard.ManaCost = CurrSerializedSpellCard.ManaCost;
		CurrSpellCard.Stats = CurrSerializedSpellCard.Stats;
		for (FString& CurrVariableInput : CurrSerializedSpellCard.VariableInputs)
		{
			CurrSpellCard.VariableInputs.Add(FName(*CurrVariableInput));
		}
		CurrSpellCard.FaceMaterial = CurrSerializedSpellCard.FaceMaterial;
		CurrSpellCard.BodyMaterial = CurrSerializedSpellCard.BodyMaterial;
		CurrSpellCard.CardEffect = CurrSerializedSpellCard.CardEffect;
		OutMap.Add(FName(*CurrSerializedSpellCard.Name), CurrSpellCard);
	}

	// Clear cache and close buffers
	FromBinary.FlushCache();
	FromBinary.Close();
	Decompressor.FlushCache();
	DecompressedBytes.Empty();
	DecompressedBytes.Close();

	return true;
}
