// ©Justin Camden 2019, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Cards/HorusCard.h"
#include "Runtime/Engine/Classes/Engine/DataTable.h"
#include "HorusSpellCard.generated.h"

class UMaterialInstance;
class UHorusSpellCardEffect;

USTRUCT(BlueprintType)
struct FHorusSpellCardData
{
	GENERATED_BODY()
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 ManaCost = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<int32> Stats;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FName> Names;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UMaterialInstance> FaceMaterial = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UMaterialInstance> BodyMaterial = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftClassPtr<UHorusSpellCardEffect> CardEffect = nullptr;
};


USTRUCT(Blueprintable)
struct FHorusSpellCardDataTableRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 ManaCost = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<int32> Stats;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FName> Names;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UMaterialInstance> FaceMaterial = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UMaterialInstance> BodyMaterial = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftClassPtr<UHorusSpellCardEffect> CardEffect = nullptr;
};

/**
 * 
 */
UCLASS(Blueprintable)
class HORUS_API UHorusSpellCard : public UHorusCard
{
	GENERATED_BODY()

public:
	// Default constructor
	UHorusSpellCard(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UPROPERTY(BlueprintReadWrite, Category = HorusSpellCard)
	FHorusSpellCardData CardData;
};
