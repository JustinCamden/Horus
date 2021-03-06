// ©Justin Camden 2019, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Horus/Arena/HorusArena.h"
#include "HorusArenaEditor.generated.h"

class UDataTable;

UCLASS()
class HORUSEDITOR_API AHorusArenaEditor : public AHorusArena
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHorusArenaEditor(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void OnConstruction(const FTransform& Transform) override;

	/*
	* Function for updating the references to zones placed with the arena.
	*/
	UFUNCTION(BlueprintCallable, Category = HorusArenaEditor)
		void MapWorldZones();

	/** Reference to the arena data table we want to pull from / export to */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UDataTable* ArenaDataTable;

	/*
	* Function for updating the references to a specific zone on the arena.
	*/
	UFUNCTION(BlueprintCallable, Category = HorusArenaEditor)
		void GetBestMappingForZone(int32 RowIdx, int32 ColumnIdx, TArray<AHorusArenaZone*>& ArenaZones);

	/** Positions associated arena zones. */
	UFUNCTION(BlueprintCallable, Category = HorusArenaEditor)
		void PositionZones();

	/** Loads data from the arena data table. */
	UFUNCTION(BlueprintCallable, Category = HorusArenaEditor)
		void ImportArenaData();

	/** Saves data from the arena into the data table. */
	UFUNCTION(BlueprintCallable, Category = HorusArenaEditor)
		void ExportArenaData();

	/*
	* Spawns arena zones according to Arena data. 
	* Disinherits any children if appropriate.
	*/
	UFUNCTION(BlueprintCallable, Category = HorusArenaEditor)
		void Editor_SpawnZones();

	/** Deletes any zones associated with this arena. */
	UFUNCTION(BlueprintCallable, Category = HorusArenaEditor)
		void DeleteChildZones();

	/** Deletes any zones not associated with this arena. */
	UFUNCTION(BlueprintCallable, Category = HorusArenaEditor)
		void DeleteOrphanZones();

	/** Deletes all zones in the world. */
	UFUNCTION(BlueprintCallable, Category = HorusArenaEditor)
		void DeleteAllZones();

	/** Resets the arena data. */
	UFUNCTION(BlueprintCallable, Category = HorusArenaEditor)
		void ClearArenaData();

	/** Detaches all child zones. */
	UFUNCTION(BlueprintCallable, Category = HorusArenaEditor)
		void DetachChildZones();
	

	/** Default class to spawn. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HorusArenaEditor)
	TSoftClassPtr<AHorusArenaZone> DefaultZoneClass;

	/** In world zones we have mapped. */
	TSet<AHorusArenaZone*> MappedZones;
};
