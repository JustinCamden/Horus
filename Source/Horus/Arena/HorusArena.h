// ©Justin Camden 2019, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HorusArena.generated.h"

class AHorusArenaZone;

#if WITH_EDITOR

class UHorusVisualBoxComponent;

#endif

UCLASS()
class HORUS_API AHorusArena : public AActor
{
	GENERATED_BODY()
	
public:	

	/** Name of the Scene Root Component. Use this name if you want to prevent creation of the component (with ObjectInitializer.DoNotCreateDefaultSubobject). */
	static FName SceneRootName;

	// Sets default values for this actor's properties
	AHorusArena(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/** Called to initialize the arena. */
	void InitializeArena();

	/** Returns an arena zone from the array. */
	UFUNCTION(BlueprintCallable, BlueprintPure)
	AHorusArenaZone* GetArenaZone(int32 Row, int32 Column) const;

	/** Used to change the size of the arena during runtime. */
	UFUNCTION(BlueprintCallable, Category = HorusArena, meta = (UnsafeDuringActorConstruction))
	void ResizeArena(int32 NewNumRows, int32 NewNumColumns);

	/** 2D Array of all the zones in the arena. */
	TArray<TArray<AHorusArenaZone*>> Zones;

	virtual void OnConstruction(const FTransform& Transform) override;

	/** Returns whether the arena has been initialized. */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = HorusArena)
		const bool IsArenaInitialized() const { return bArenaInitialized; }

	/** Called after the arena has been fully initialized. */
	UFUNCTION(BlueprintImplementableEvent, Category = HorusArena)
		void OnArenaInitialized();


#if WITH_EDITOR
	/*
	* Function for updating any zones already in the arena.
	* This should always be run before starting the game and after all updates have been made.
	*/
	UFUNCTION(BlueprintCallable, Category = HorusArena)
		void UpdateZoneMappings();
#endif

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


#if WITH_EDITOR
	// Visualization components for the zones
	TArray<TArray<UHorusVisualBoxComponent*>> ZoneVisualizations;

	// Height of the visualization zones.
	UPROPERTY(EditAnywhere, Category = HorusArena)
		float ZoneVisHeight;

	// Line thickness of the visualization zones.
	UPROPERTY(EditAnywhere, Category = HorusArena)
		float ZoneVisLineThickness;


#endif

private:
	/** Whether the arena has been initialized. */
		bool bArenaInitialized;

	/** Used to generate an arena according to the number of rows and columns. */
	void ConstructArena();

	/** Root scene component. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = HorusArenaZoneGenerator, meta = (AllowPrivateAccess = "true"))
		USceneComponent* SceneRoot;
};
