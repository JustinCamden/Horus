// Fill out your copyright notice in the Description page of Project Settings.


#include "HorusArena.h"
#include "HorusArenaZone.h"
#include "Runtime/Engine/Classes/Engine/World.h"

#if WITH_EDITOR

#include "Core/HorusVisualBoxComponent.h"

#endif



// Sets default values
AHorusArena::AHorusArena()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;
	NumColumns = 3;
	NumRows = 4;
	ColumnWidth = 50.0f;
	RowLength = 75.0f;
	ZoneSpawnHeightOffset = 0.0f;
	WidthBetweenColumns = 10.0f;
	LengthBetweenRows = 10.0f;
	ArenaHalfLength = 0.0f;
	ArenaHalfWidth = 0.0f;

	if (!RootComponent)
	{
		RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultRoot"));
	}
#if WITH_EDITOR
	ZoneVisHeight = 50.0f;
	ZoneVisLineThickness = 5.0f;
#endif

}

// Called when the game starts or when spawned
void AHorusArena::BeginPlay()
{
	Super::BeginPlay();

#if WITH_EDITOR
	// Remove all visualization components
	for(TArray<UHorusVisualBoxComponent*>& VisRow : ZoneVisualizations)
	{
		for (UHorusVisualBoxComponent*& CurrVis : VisRow)
		{
			CurrVis->SetGenerateOverlapEvents(false);
			CurrVis->DestroyComponent();
			CurrVis = nullptr;
		}
	}
#endif
}

void AHorusArena::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	ConstructArena();
}

// Called every frame
void AHorusArena::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AHorusArena::InitializeArena()
{
	// Initialize spawning variables
	FVector BaseSpawnLoc = GetActorLocation();

	BaseSpawnLoc.X -= ArenaHalfLength;
	BaseSpawnLoc.Y -= ArenaHalfWidth;
	BaseSpawnLoc.Z = ZoneSpawnHeightOffset;
	FVector SpawnOffset = FVector::ZeroVector;
	FRotator SpawnRotaion = GetActorRotation();
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	// Create and populate each row
	for (int32 ColumnIdx = 0; ColumnIdx < NumColumns; ColumnIdx++)
	{
		// Create and populate each column
		SpawnOffset.Y = (float)ColumnIdx * ColumnWidth + (float)ColumnIdx - 1.0f * WidthBetweenColumns;
		BaseSpawnLoc.X = 0.0f;
		for (int32 RowIdx = 0; RowIdx < NumRows; RowIdx++)
		{

		}
	}
}

AHorusArenaZone* AHorusArena::GetArenaZone(int32 Column, int32 Row) const
{
	if (Column < NumColumns && Row < NumRows)
	{
		return Zones[Column][Row];
	}
	return nullptr;
}

void AHorusArena::ConstructArena()
{
	// Initialize arena dimensions
	Zones.SetNum(NumRows);
	for (int32 Idx = 0; Idx < NumRows; Idx++)
	{
		Zones[Idx].SetNum(NumColumns);
	}

	// Calculate the new length and width
	float ArenaLength = (float)NumRows;
	float ArenaWidth = (float)NumColumns;
	ArenaLength = (ArenaLength* RowLength) + (NumRows > 1? ((ArenaLength - 1.0f) * LengthBetweenRows) : 0.0f);
	ArenaWidth = (ArenaWidth * ColumnWidth) + (NumColumns > 1? ((ArenaWidth - 1.0f) * WidthBetweenColumns) : 0.0f);
	ArenaHalfLength = ArenaLength * 0.5f;
	ArenaHalfWidth = ArenaWidth * 0.5f;

#if WITH_EDITOR
	if (!GetWorld()->HasBegunPlay())
	{
		// Clean up old vis comps
		if (NumRows < ZoneVisualizations.Num())
		{
			for (int32 Idx = NumRows; Idx < ZoneVisualizations.Num(); Idx++)
			{
				TArray<UHorusVisualBoxComponent*>& CurrRow = ZoneVisualizations[Idx];
				for (UHorusVisualBoxComponent*& CurrVis : CurrRow)
				{
					CurrVis->DestroyComponent();
					CurrVis = nullptr;
				}
			}
		}
		ZoneVisualizations.SetNum(NumRows);

		if (NumColumns < ZoneVisualizations[0].Num())
		{
			for (int32 RowIdx = 0; RowIdx < NumRows; RowIdx++)
			{
				TArray<UHorusVisualBoxComponent*>& CurrRow = ZoneVisualizations[RowIdx];
				for (int32 ColumnIdx = NumColumns; ColumnIdx < CurrRow.Num(); ColumnIdx++)
				{
					UHorusVisualBoxComponent*& CurrVis = ZoneVisualizations[RowIdx][ColumnIdx];
					CurrVis->DestroyComponent();
					CurrVis = nullptr;
				}
			}
		}
		for (int32 Idx = 0; Idx < NumRows; Idx++)
		{
			ZoneVisualizations[Idx].SetNum(NumColumns);
		}


		// Create visualization components
		FTransform SpawnTransform;
		FVector BaseSpawnLoc = FVector(-ArenaHalfLength, -ArenaHalfWidth, ZoneSpawnHeightOffset + ZoneVisHeight * 0.5f);
		FVector SpawnOffset = FVector::ZeroVector;
		FRotator SpawnRotaion = GetActorRotation();
		FAttachmentTransformRules AttachmentRules(EAttachmentRule::KeepWorld, false);

		// Create and populate each row
		for (int32 Idx = 0; Idx < NumRows; Idx++)
		{
			SpawnOffset.X = ((float)Idx / NumRows) * ArenaLength;

			// Create and populate each column
			for (int32 ColumnIdx = 0; ColumnIdx < NumColumns; ColumnIdx++)
			{
				SpawnOffset.Y = ((float)ColumnIdx / NumColumns) * ArenaWidth;
				SpawnTransform.SetLocation(BaseSpawnLoc + SpawnOffset);
				UHorusVisualBoxComponent*& CurrVis = ZoneVisualizations[Idx][ColumnIdx];
				if (!CurrVis)
				{
					CurrVis = NewObject<UHorusVisualBoxComponent>(this);
					CurrVis->RegisterComponent();
					CurrVis->AttachToComponent(RootComponent, AttachmentRules);
				}
				CurrVis->SetRelativeTransform(SpawnTransform);
				CurrVis->SetBoxExtent(FVector(RowLength * 0.5f, ColumnWidth * 0.5f, ZoneVisHeight * 0.5f));
				CurrVis->SetLineThickness(ZoneVisLineThickness);
			}
		}
	}
#endif

	return;
}

void AHorusArena::ResizeArena(int32 NewNumColumns, int32 NewNumRows)
{
	// Initialize new arena dimensions
	int32 OldNumColumns = NumColumns;
	int32 OldNumRows = NumRows;
	NumColumns = FMath::Max(1, NewNumColumns);
	NumRows = FMath::Max(1, NewNumRows);

	// If the size of the map changed
	if (OldNumColumns != NumColumns || OldNumRows != NumRows)
	{
		// Prune extra rows
		for (int32 Idx = NumRows; Idx < OldNumRows; Idx++)
		{
			TArray<AHorusArenaZone*>& CurrRow = Zones[Idx];
			for (AHorusArenaZone*& CurrZone : CurrRow)
			{
				if (CurrZone)
				{
					CurrZone->SetLifeSpan(0.00001f);
					CurrZone = nullptr;
				}
			}
		}

		// Prune extra columns
		for (int32 RowIdx = 0; RowIdx < NumRows; RowIdx++)
		{
			for (int32 ColumnIdx = NumColumns; ColumnIdx < OldNumColumns; ColumnIdx++)
			{
				AHorusArenaZone*& CurrZone = Zones[RowIdx][ColumnIdx];
				if (CurrZone)
				{
					CurrZone->SetLifeSpan(0.00001f);
					CurrZone = nullptr;
				}
			}
		}

		// Rebuild the arena
		ConstructArena();

#if WITH_EDITOR
		// Cleanup old visualization components if necessary.
#endif
	}

	return;
}

