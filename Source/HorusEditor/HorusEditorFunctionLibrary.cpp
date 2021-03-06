// ©Justin Camden 2019, all rights reserved.


#include "HorusEditorFunctionLibrary.h"
#include "Horus/Cards/HorusCardFileHelperLibrary.h"
#include "Runtime/Core/Public/Serialization/MemoryReader.h"
#include "Runtime/Core/Public/Serialization/ArchiveLoadCompressedProxy.h"
#include "Runtime/Core/Public/Serialization/BufferArchive.h"
#include "Runtime/Core/Public/Misc/FileHelper.h"
#include "Runtime/Core/Public/Serialization/ArchiveSaveCompressedProxy.h"
#include "DataTools/HorusBufferArchive.h"

DEFINE_LOG_CATEGORY_STATIC(LogHorusEditorFunctionLibrary, All, All);

bool UHorusEditorFunctionLibrary::SaveSpellCardDataTableToCompressedBinaryFile(const UDataTable* DataTable, const FString& FilePath)
{
	// Check if data table is valid
	if (!DataTable)
	{
		UE_LOG(LogHorusEditorFunctionLibrary, Error, TEXT("ERROR attempting to save file as a compressed spell card binary file: Data Table was not valid!"));
		return false;
	}

	// Check if the data table is the right type
	if (DataTable->GetRowStructName().ToString() != "HorusSpellCardDataTableRow")
	{
		UE_LOG(LogHorusEditorFunctionLibrary, Error, TEXT("ERROR attempting to save %s as a compressed spell card binary file: Data Table row struct was not HorusSpellCardDataTableRow! Was %s."), *DataTable->GetRowStructName().ToString());
		return false;
	}

	// Convert data table to an array of serializable data
	auto& RowMap = DataTable->GetRowMap();
	TArray<FHorusSpellCardDataSerializable> SerializedRows;
	SerializedRows.Reserve(RowMap.Num());
	for (auto& CurrRow : RowMap)
	{
		FHorusSpellCardDataTableRow CurrRowData = *reinterpret_cast<FHorusSpellCardDataTableRow*>(CurrRow.Value);
		FHorusSpellCardDataSerializable CurrSerializedRow;
		CurrSerializedRow.Name = CurrRow.Key.ToString();
		CurrSerializedRow.ManaCost = CurrRowData.ManaCost;
		CurrSerializedRow.Stats = CurrRowData.Stats;
		CurrSerializedRow.VariableInputs = CurrRowData.VariableInputs;
		CurrSerializedRow.FaceMaterial = CurrRowData.FaceMaterial;
		CurrSerializedRow.BodyMaterial = CurrRowData.BodyMaterial;
		CurrSerializedRow.CardEffect = CurrRowData.CardEffect;
		SerializedRows.Add(CurrSerializedRow);
	}

	// Convert the spell card data to bytes
	FHorusBufferArchive ToBinary;
	ToBinary << SerializedRows;

	// Compress file
	TArray<uint8> CompressedBytes;
	FArchiveSaveCompressedProxy Compressor = FArchiveSaveCompressedProxy(CompressedBytes, NAME_Zlib, ECompressionFlags::COMPRESS_NoFlags);
	Compressor << ToBinary;
	Compressor.Flush();

	// Save the file
	if (!FFileHelper::SaveArrayToFile(CompressedBytes, *FilePath))
	{
		UE_LOG(LogHorusEditorFunctionLibrary, Error, TEXT("ERROR: Unable to save %s!"), *FilePath);

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

bool UHorusEditorFunctionLibrary::SaveArenaDataTableToCompressedBinaryFile(const UDataTable* DataTable, const FString& FilePath)
{
	// Check if data table is valid
	if (!DataTable)
	{
		UE_LOG(LogHorusEditorFunctionLibrary, Error, TEXT("ERROR attempting to save file as a compressed arena binary file: Data Table was not valid!"));
		return false;
	}

	// Check if the data table is the right type
	if (DataTable->GetRowStructName().ToString() != "HorusArenaDataTableRow")
	{
		UE_LOG(LogHorusEditorFunctionLibrary, Error, TEXT("ERROR attempting to save %s as a compressed arena binary file: Data Table row struct was not HorusArenaDataTableRow! Was %s."), *DataTable->GetRowStructName().ToString());
		return false;
	}

	// Convert data table to an array of serializable data
	auto& RowMap = DataTable->GetRowMap();
	TArray<FHorusArenaDataSerializable> SerializedRows;
	SerializedRows.Reserve(RowMap.Num());
	for (auto& CurrRow : RowMap)
	{
		FHorusArenaDataTableRow CurrRowData = *reinterpret_cast<FHorusArenaDataTableRow*>(CurrRow.Value);
		FHorusArenaDataSerializable CurrSerializedRow;
		CurrSerializedRow.Name = CurrRow.Key.ToString();
		CurrSerializedRow.DefaultZoneClass = CurrRowData.DefaultZoneClass;
		CurrSerializedRow.Rows = CurrRowData.Rows;
		SerializedRows.Add(CurrSerializedRow);
	}

	// Convert the spell card data to bytes
	FHorusBufferArchive ToBinary;
	ToBinary << SerializedRows;

	// Compress file
	TArray<uint8> CompressedBytes;
	FArchiveSaveCompressedProxy Compressor = FArchiveSaveCompressedProxy(CompressedBytes, NAME_Zlib, ECompressionFlags::COMPRESS_NoFlags);
	Compressor << ToBinary;
	Compressor.Flush();

	// Save the file
	if (!FFileHelper::SaveArrayToFile(CompressedBytes, *FilePath))
	{
		UE_LOG(LogHorusEditorFunctionLibrary, Error, TEXT("ERROR: Unable to save %s!"), *FilePath);

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
