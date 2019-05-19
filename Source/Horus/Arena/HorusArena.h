// Fill out your copyright notice in the Description page of Project Settings.

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
	// Sets default values for this actor's properties
	AHorusArena();

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

	virtual void OnConstruction(const FTransform& Transform) override;

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

	/** 2D Array of all the zones in the arena. */
	TArray<TArray<AHorusArenaZone*>> Zones;

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

	/** Used to generate an arena according to the number of rows and columns. */
	void ConstructArena();


};
