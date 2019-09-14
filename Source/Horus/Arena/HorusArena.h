// ©Justin Camden 2019, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HorusArena.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogHorusArena, Log, All);

class AHorusArenaZone;

#if WITH_EDITORONLY_DATA

class UHorusVisBoxComponent;

#endif

USTRUCT(Blueprintable)
struct FHorusArenaRowData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<TSoftClassPtr<AHorusArenaZone>> Columns;
};

FORCEINLINE FArchive& operator<<(FArchive& Ar, FHorusArenaRowData& ArenaRowData)
{
	Ar << ArenaRowData.Columns;
	return Ar;
}

USTRUCT(Blueprintable)
struct FHorusArenaData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftClassPtr<AHorusArenaZone> DefaultZoneClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FHorusArenaRowData> Rows;
};

USTRUCT(Blueprintable)
struct FHorusArenaRow
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<AHorusArenaZone*> Columns;
};


UCLASS()
class HORUS_API AHorusArena : public AActor
{
	GENERATED_BODY()
	
public:	

	/** Name of the Scene Root Component. Use this name if you want to prevent creation of the component (with ObjectInitializer.DoNotCreateDefaultSubobject). */
	static FName SceneRootName;

	// Sets default values for this actor's properties
	AHorusArena(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/** Called to initialize the arena. */
	void InitializeArena();

	/** Returns an arena zone from the array. */
	UFUNCTION(BlueprintCallable, BlueprintPure)
	AHorusArenaZone* GetArenaZone(int32 Row, int32 Column) const;

	/** 2D Array of all the zones in the arena. */
	UPROPERTY(BlueprintReadOnly)
	TArray<FHorusArenaRow> Rows;

#if WITH_EDITOR

	virtual void OnConstruction(const FTransform& Transform) override;

#endif

	/** Returns whether the arena has been initialized. */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = HorusArena)
		const bool IsArenaInitialized() const { return bArenaInitialized; }

	/** Called after the arena has been fully initialized. */
	UFUNCTION(BlueprintImplementableEvent, Category = HorusArena)
		void OnArenaInitialized();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/**  Number of zones wide. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = HorusArena, meta = (ClampMin = "1"))
		int32 NumColumns;

	/** Number of zones long. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = HorusArena, meta = (ClampMin = "1"))
		int32 NumRows;

	/** Width of the zone actors */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = HorusArena)
		float ColumnWidth;

	/** Length of the zone actors. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = HorusArena)
		float RowLength;

	/** Height offset for zone actors. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = HorusArena)
		float ZoneSpawnHeightOffset;

	/** Width space between the zone actors. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = HorusArena)
		float WidthBetweenColumns;

	/** Width length between the zone actors. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = HorusArena)
		float LengthBetweenRows;

	/** Half length of the arena. */
	UPROPERTY(BlueprintReadOnly, Category = HorusArena)
		float ArenaHalfLength;

	/** Half width of the arena. */
	UPROPERTY(BlueprintReadOnly, Category = HorusArena)
		float ArenaHalfWidth;

	/** Calculates and lays out the shape of the arena, according to the number of rows and columns. */
	void LayoutArena();

	/** Spawns and places Arena Zones according to Arena Data. */
	void SpawnZones();

	/** Name of the arena data to load. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName ArenaName;

	/** Loaded arena data */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = HorusArena)
		FHorusArenaData ArenaData;

private:
	/** Whether the arena has been initialized. */
	bool bArenaInitialized;



	/** Root scene component. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = HorusArenaZoneGenerator, meta = (AllowPrivateAccess = "true"))
		USceneComponent* SceneRoot;

#if WITH_EDITORONLY_DATA

public:
	// Visualization components for the zones
	TArray<TArray<UHorusVisBoxComponent*>> ZoneVisualizations;

	// Height of the visualization zones.
	UPROPERTY(EditAnywhere, Category = HorusArena)
		float ZoneVisHeight;

	// Line thickness of the visualization zones.
	UPROPERTY(EditAnywhere, Category = HorusArena)
		float ZoneVisLineThickness;


#endif
};
