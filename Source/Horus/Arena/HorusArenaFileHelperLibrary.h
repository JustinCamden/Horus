// ©Justin Camden 2019, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Arena/HorusArena.h"
#include "HorusArenaFileHelperLibrary.generated.h"

class AHorusArenaZone;


USTRUCT(Blueprintable)
struct FHorusArenaDataSerializable
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString Name = "";

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftClassPtr<AHorusArenaZone> DefaultZoneClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FHorusArenaRowData> Rows;
};

FORCEINLINE FArchive& operator<<(FArchive& Ar, FHorusArenaDataSerializable& ArenaDataSerializable)
{
	Ar << ArenaDataSerializable.Name;
	Ar << ArenaDataSerializable.DefaultZoneClass;
	Ar << ArenaDataSerializable.Rows;
	return Ar;
}

/**
 * Library of functions for loading and managing arena data from files.
 */
UCLASS()
class HORUS_API UHorusArenaFileHelperLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	/** Decompresses a binary file and loads it as a map of ArenaData structs. */
	UFUNCTION(BlueprintCallable, Category = HorusSpellCardFileHelperLibrary)
		static bool LoadCompressedBinaryFileToArenaMap(const FString& FilePath, TMap<FName, FHorusArenaData>& OutMap, bool bAppendToMap);

};
