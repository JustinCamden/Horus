// �Justin Camden 2019, all rights reserved.

#include "HorusArena.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "ArenaZones/HorusArenaZone.h"
#include "HorusArenaSubsystem.h"

#if WITH_EDITOR

#include "Core/Tools/HorusVisBoxComponent.h"

#endif

// Log categories
DEFINE_LOG_CATEGORY(LogHorusArena)

FName AHorusArena::SceneRootName(TEXT("SceneRoot"));


// Sets default values
AHorusArena::AHorusArena(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;
	NumColumns = 3;
	NumRows = 4;
	ColumnWidth = 75.0f;
	RowLength = 100.0f;
	ZoneSpawnHeightOffset = 0.0f;
	WidthBetweenColumns = 25.0f;
	LengthBetweenRows = 25.0f;
	ArenaHalfLength = 0.0f;
	ArenaHalfWidth = 0.0f;
	bArenaInitialized = false;
	ArenaName = NAME_None;

	// Initialize components
	SceneRoot = CreateDefaultSubobject<USceneComponent>(SceneRootName);
	RootComponent = SceneRoot;

#if WITH_EDITOR
	ZoneVisHeight = 50.0f;
	ZoneVisLineThickness = 3.0f;
#endif

}

// Called when the game starts or when spawned
void AHorusArena::BeginPlay()
{
	Super::BeginPlay();

#if WITH_EDITOR
	// Remove all visualization components
	for(TArray<UHorusVisBoxComponent*>& VisRow : ZoneVisualizations)
	{
		for (UHorusVisBoxComponent*& CurrVis : VisRow)
		{
			CurrVis->SetGenerateOverlapEvents(false);
			CurrVis->DestroyComponent();
			CurrVis = nullptr;
		}
	}
#endif
}

#if WITH_EDITOR

void AHorusArena::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	LayoutArena();
}

#endif

void AHorusArena::InitializeArena()
{
	LoadArenaData();
	LayoutArena();
	SpawnZones();

	// Call initialized on the arena and all zones
	OnArenaInitialized();
	bArenaInitialized = true;
	for (FHorusArenaRow& CurrRow : Rows)
	{
		for (AHorusArenaZone* CurrColumn : CurrRow.Columns)
		{
			CurrColumn->OnArenaInitialized();
		}
	}

	UE_LOG(LogHorusArena, Display, TEXT("Arena %s initialized"), *GetNameSafe(this));

	return;
}

AHorusArenaZone* AHorusArena::GetArenaZone(int32 Row, int32 Column) const
{
	if (Column < NumColumns && Row < NumRows)
	{
		return Rows[Row].Columns[Column];
	}
	return nullptr;
}

void AHorusArena::LoadArenaData()
{
	UHorusArenaSubsystem* ArenaSubSystem = GetWorld()->GetGameInstance()->GetSubsystem<UHorusArenaSubsystem>();
	check(ArenaSubSystem && "ERROR attempting to load arena data: Arena Subsystem was nullptr");
	checkf(ArenaSubSystem->GetArenaDatabase().Contains(ArenaName), TEXT("ERROR Attempting to load arena data: Arena Database did not contain entry for %s!"), *ArenaName.ToString());
	ArenaData = ArenaSubSystem->GetArenaDatabase()[ArenaName];
	NumRows = ArenaData.Rows.Num();
	NumColumns = ArenaData.Rows[0].Columns.Num();

	return;
}


void AHorusArena::LayoutArena()
{
	// Initialize arena dimensions
	Rows.SetNum(NumRows);
	for (int32 Idx = 0; Idx < NumRows; Idx++)
	{
		Rows[Idx].Columns.SetNum(NumColumns);
	}

	// Calculate the new length and width
	float ArenaLength = (float)NumRows;
	float ArenaWidth = (float)NumColumns;
	ArenaLength = (ArenaLength * RowLength) + (NumRows > 1? ((ArenaLength - 1.0f) * LengthBetweenRows) : 0.0f);
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
				TArray<UHorusVisBoxComponent*>& CurrRow = ZoneVisualizations[Idx];
				for (UHorusVisBoxComponent*& CurrVis : CurrRow)
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
				TArray<UHorusVisBoxComponent*>& CurrRow = ZoneVisualizations[RowIdx];
				for (int32 ColumnIdx = NumColumns; ColumnIdx < CurrRow.Num(); ColumnIdx++)
				{
					UHorusVisBoxComponent*& CurrVis = ZoneVisualizations[RowIdx][ColumnIdx];
					CurrVis->DestroyComponent();
					CurrVis = nullptr;
				}
			}
		}
		for (int32 RowIdx = 0; RowIdx < NumRows; RowIdx++)
		{
			ZoneVisualizations[RowIdx].SetNum(NumColumns);
		}


		// Create visualization components
		FTransform SpawnTransform;
		FVector BaseSpawnLoc = FVector(-ArenaHalfLength + (0.5f * RowLength), -ArenaHalfWidth + (0.5f * ColumnWidth), ZoneSpawnHeightOffset + ZoneVisHeight * 0.5f);
		FVector SpawnOffset = FVector::ZeroVector;
		FAttachmentTransformRules AttachmentRules(EAttachmentRule::KeepWorld, false);

		// Create and populate each row
		for (int32 RowIdx = 0; RowIdx < NumRows; RowIdx++)
		{
			SpawnOffset.X = ((float)RowIdx / NumRows) * ArenaLength;

			// Create and populate each column
			for (int32 ColumnIdx = 0; ColumnIdx < NumColumns; ColumnIdx++)
			{
				SpawnOffset.Y = ((float)ColumnIdx / NumColumns) * ArenaWidth;
				SpawnTransform.SetLocation(BaseSpawnLoc + SpawnOffset);
				UHorusVisBoxComponent*& CurrVis = ZoneVisualizations[RowIdx][ColumnIdx];
				if (!CurrVis)
				{
					CurrVis = NewObject<UHorusVisBoxComponent>(this);
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

void AHorusArena::SpawnZones()
{
	// Initialize variables
	FVector BaseSpawnLoc = FVector(-ArenaHalfLength + (0.5f * RowLength), -ArenaHalfWidth + (0.5f * ColumnWidth), ZoneSpawnHeightOffset);
	FVector SpawnOffset = FVector::ZeroVector;
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Owner = this;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	FAttachmentTransformRules SpawnAttachmentRules = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, false);
	
	// Create and populate each row
	checkf(ArenaData.Rows.Num() == NumRows, TEXT("%s: ERROR attempting to spawn Arena Zones: Num Rows does not match Arena Data!"), *GetNameSafe(this));
	for (int32 RowIdx = 0; RowIdx < NumRows; RowIdx++)
	{
		checkf(ArenaData.Rows[0].Columns.Num() == NumColumns, TEXT("%s: ERROR attempting to spawn Arena Zones: Num Columns does not match Arena Data Row %d!"), *GetNameSafe(this), RowIdx);
		SpawnOffset.X = ((float)RowIdx / NumRows) * ArenaHalfLength * 2.0f;

		// Create and populate each column
		for (int32 ColumnIdx = 0; ColumnIdx < NumColumns; ColumnIdx++)
		{
			// Get the class
			TSubclassOf<AHorusArenaZone> ZoneClass = ArenaData.Rows[RowIdx].Columns[ColumnIdx].Get();
			if (!ZoneClass)
			{
				ZoneClass = ArenaData.DefaultZoneClass.Get();
			}

			// If there is a valid zone class, spawn the zone
			if (ZoneClass)
			{
				SpawnOffset.Y = ((float)ColumnIdx / NumColumns) * ArenaHalfWidth * 2.0f;
				AHorusArenaZone* NewZone = GetWorld()->SpawnActor<AHorusArenaZone>(ArenaData.DefaultZoneClass.Get(), FTransform(), SpawnParameters);
				NewZone->Arena = this;
				NewZone->RowIdx = RowIdx;
				NewZone->ColumnIdx = ColumnIdx;
				NewZone->AttachToActor(this, SpawnAttachmentRules);
				NewZone->SetActorRelativeLocation(BaseSpawnLoc + SpawnOffset);
				Rows[RowIdx].Columns[ColumnIdx] = NewZone;
			}
			else
			{
				UE_LOG(LogHorusArena, Warning, TEXT("%s: Warning attempting to spawn zone for Row %d Column %: No valid zone class found. No zone will be spawned."), *GetNameSafe(this), RowIdx, ColumnIdx);
			}
		}
	}

	MapZoneNeighbors();

	return;
}

void AHorusArena::MapZoneNeighbors()
{
	for (int32 RowIdx = 0; RowIdx < NumRows; RowIdx++)
	{
		for (int32 ColumnIdx = 0; ColumnIdx < NumColumns; ColumnIdx++)
		{
			AHorusArenaZone*& CurrZone = Rows[RowIdx].Columns[ColumnIdx];
			if (CurrZone)
			{
				if (NumRows > 1)
				{
					// Set next row
					if (RowIdx < NumRows - 1)
					{
						CurrZone->NextRowZone = Rows[RowIdx + 1].Columns[ColumnIdx];
					}

					// Set previous row
					if (RowIdx > 0)
					{
						CurrZone->PreviousRowZone = Rows[RowIdx - 1].Columns[ColumnIdx];
					}
				}

				if (NumColumns > 1)
				{
					// Set next column
					if (ColumnIdx < NumColumns - 1)
					{
						CurrZone->NextColumnZone = Rows[RowIdx].Columns[ColumnIdx + 1];
					}

					// Set previous column
					if (ColumnIdx > 0)
					{
						CurrZone->PreviousColumnZone = Rows[RowIdx].Columns[ColumnIdx - 1];
					}
				}
			}
		}
	}

	return;
}

