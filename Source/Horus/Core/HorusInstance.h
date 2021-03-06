// ©Justin Camden 2019, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintPlatformLibrary.h"
#include "HorusInstance.generated.h"

class UDataTable;

/**
 * Game instance class for managing and holding persistant data
 */
UCLASS()
class HORUS_API UHorusInstance : public UPlatformGameInstance
{
	GENERATED_BODY()
	
public:

	// Default constructor
	UHorusInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

#if WITH_EDITORONLY_DATA
	/*
	* List of spell card data tables to compile into the database
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = HorusInstance)
	TArray<TSoftObjectPtr<UDataTable>> SpellCardDatabaseList;
#endif

};
