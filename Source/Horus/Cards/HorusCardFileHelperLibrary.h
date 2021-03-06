// ©Justin Camden 2019, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "HorusSpellCard.h"
#include "HorusCardFileHelperLibrary.generated.h"

USTRUCT(Blueprintable)
struct FHorusSpellCardDataSerializable
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString Name = "";

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

FORCEINLINE FArchive& operator<<(FArchive& Ar, FHorusSpellCardDataSerializable& SpellCardData)
{
	Ar << SpellCardData.Name;
	Ar << SpellCardData.ManaCost;
	Ar << SpellCardData.Stats;
	Ar << SpellCardData.VariableInputs;
	Ar << SpellCardData.FaceMaterial;
	Ar << SpellCardData.BodyMaterial;
	Ar << SpellCardData.CardEffect;

	return Ar;
}

/*
 * Holds functions and structs for managing card data loaded from files.
 */

UCLASS()
class HORUS_API UHorusCardFileHelperLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:

	/** Decompresses a binary file and loads it as a map of SpellCardData structs. */
	UFUNCTION(BlueprintCallable, Category = HorusSpellCardFileHelperLibrary)
		static bool LoadCompressedBinaryFileToSpellCardMap(const FString& FilePath, TMap<FName, FHorusSpellCardData>& OutMap, bool bAppendDataToMap);


};
