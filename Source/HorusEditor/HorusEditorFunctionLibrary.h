// ©Justin Camden 2019, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Runtime/Engine/Classes/Engine/DataTable.h"
#include "Arena/HorusArenaFileHelperLibrary.h"
#include "HorusEditorFunctionLibrary.generated.h"

/*
 * Forward declarations.
 */
class UMaterialInstance;
class UHorusEffect;
class AHorusArenaZone;

/*
 * Helper structs.
 */

USTRUCT(Blueprintable)
struct FHorusSpellCardDataTableRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 ManaCost = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<int32> Stats;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FString> VariableInputs;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UMaterialInstance> FaceMaterial = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UMaterialInstance> BodyMaterial = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftClassPtr<UHorusEffect> CardEffect = nullptr;
};

USTRUCT(Blueprintable)
struct FHorusArenaDataTableRow : public FTableRowBase
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftClassPtr<AHorusArenaZone> DefaultZoneClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FHorusArenaRowData> Rows;
};

/*
 * Contains editor only functions useful for scripting editor behavior.
 */
UCLASS()
class HORUSEDITOR_API UHorusEditorFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/*
	* Saves a spell card data table to a compressed binary file.
	*/
	UFUNCTION(BlueprintCallable, Category = HorusFileHelperLibrary)
		static bool SaveSpellCardDataTableToCompressedBinaryFile(const UDataTable* DataTable, const FString& FilePath);

	/*
	* Saves an arena data table to a compressed binary file.
	*/
	UFUNCTION(BlueprintCallable, Category = HorusFileHelperLibrary)
		static bool SaveArenaDataTableToCompressedBinaryFile(const UDataTable* DataTable, const FString& FilePath);

};
