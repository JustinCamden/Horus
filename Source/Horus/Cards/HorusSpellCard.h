// ©Justin Camden 2019, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Cards/HorusCard.h"
#include "HorusSpellCard.generated.h"

class UMaterialInstance;
class UHorusEffect;

USTRUCT(BlueprintType)
struct FHorusSpellCardData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 ManaCost = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<int32> Stats;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FName> VariableInputs;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UMaterialInstance> FaceMaterial = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UMaterialInstance> BodyMaterial = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftClassPtr<UHorusEffect> CardEffect = nullptr;
};


/**
 * Class containing functions pertaining to an individual spell card.
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
