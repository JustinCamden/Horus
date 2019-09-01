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
		FString Name;

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

//Make as many Unique Overloads as you want!
FORCEINLINE FArchive& operator<<(FArchive& Ar, FHorusSpellCardDataSerializable& SpellCardData)
{
	Ar << SpellCardData.Name;
	Ar << SpellCardData.ManaCost;
	Ar << SpellCardData.Stats;
	Ar << SpellCardData.VariableInputs;

	// For the editor, we need to support two way data editing since we use it to build the binary files.
	// For runtime however, we only ever will be loading card data, as any saving should be done as bytes,
	// and never directly from living card data.
#if WITH_EDITOR
	FSoftObjectPath Path = SpellCardData.FaceMaterial.ToSoftObjectPath();
	Ar = FArchiveUObject::SerializeSoftObjectPath(Ar, Path);
	SpellCardData.FaceMaterial = Path;
	Path = SpellCardData.BodyMaterial.ToSoftObjectPath();
	Ar = FArchiveUObject::SerializeSoftObjectPath(Ar, Path);
	SpellCardData.BodyMaterial = Path;
	Path = SpellCardData.CardEffect.ToSoftObjectPath();
	Ar = FArchiveUObject::SerializeSoftObjectPath(Ar, Path);
	SpellCardData.CardEffect = Path;
#else
	FSoftObjectPath Path;
	Ar = FArchiveUObject::SerializeSoftObjectPath(Ar, Path);
	SpellCardData.FaceMaterial = Path;
	Ar = FArchiveUObject::SerializeSoftObjectPath(Ar, Path);
	SpellCardData.BodyMaterial = Path;
	Ar = FArchiveUObject::SerializeSoftObjectPath(Ar, Path);
	SpellCardData.CardEffect = Path;
#endif


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

	UFUNCTION(BlueprintCallable, Category = HorusSpellCardFileHelperLibrary)
	static bool LoadCompressedBinaryFileToSpellCardMap(const FString& FilePath, TMap<FName, FHorusSpellCardData>& OutMap);


};
