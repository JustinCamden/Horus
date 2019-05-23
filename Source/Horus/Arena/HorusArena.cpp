// ©Justin Camden 2019, all rights reserved.

#include "HorusArena.h"
#include "HorusArenaZone.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

#if WITH_EDITOR

#include "Core/HorusVisualBoxComponent.h"

#endif

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

	// Initialize components
	SceneRoot = CreateDefaultSubobject<USceneComponent>(SceneRootName);
	RootComponent = SceneRoot;

#if WITH_EDITOR
	ZoneVisHeight = 50.0f;
	ZoneVisLineThickness = 2.0f;
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
	FTransform SpawnTransform;
	SpawnTransform.SetRotation(GetActorTransform().GetRotation());
	FVector BaseSpawnLoc = FVector(-ArenaHalfLength + (0.5f * RowLength), -ArenaHalfWidth + (0.5f * ColumnWidth), ZoneSpawnHeightOffset) + GetActorLocation();
	FVector SpawnOffset = FVector::ZeroVector;
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	// Create and populate each row
	for (int32 RowIdx = 0; RowIdx < NumRows; RowIdx++)
	{
		SpawnOffset.X = ((float)RowIdx / NumRows) * ArenaHalfLength * 2.0f;

		// Create and populate each column
		for (int32 ColumnIdx = 0; ColumnIdx < NumColumns; ColumnIdx++)
		{
			AHorusArenaZone*& CurrZone = Zones[RowIdx][ColumnIdx];

			// Spawn new zone if necessary
			if (!CurrZone)
			{
				SpawnOffset.Y = ((float)ColumnIdx / NumColumns) * ArenaHalfWidth * 2.0f;
				SpawnTransform.SetLocation(BaseSpawnLoc + SpawnOffset);
				AHorusArenaZone* NewZone = GetWorld()->SpawnActor<AHorusArenaZone>(AHorusArenaZone::StaticClass(), SpawnTransform, SpawnParameters);
				NewZone->Arena = this;
				NewZone->RowIdx = RowIdx;
				NewZone->ColumnIdx = ColumnIdx;
				CurrZone = NewZone;
			}
		}
	}

	// After spawning each zone, update zone neighbor mappings
	for (int32 RowIdx = 0; RowIdx < NumRows; RowIdx++)
	{
		for (int32 ColumnIdx = 0; ColumnIdx < NumColumns; ColumnIdx++)
		{
			AHorusArenaZone*& CurrZone = Zones[RowIdx][ColumnIdx];

			if (NumRows > 1)
			{
				// Set next row
				if (RowIdx < NumRows - 1)
				{
					CurrZone->NextRowZone = Zones[RowIdx + 1][ColumnIdx];
				}

				// Set previous row
				if (RowIdx > 0)
				{
					CurrZone->PreviousRowZone = Zones[RowIdx - 1][ColumnIdx];
				}
			}

			if (NumColumns > 1)
			{
				// Set next column
				if (ColumnIdx < NumColumns - 1)
				{
					CurrZone->NextColumnZone = Zones[RowIdx][ColumnIdx + 1];
				}

				// Set previous column
				if (ColumnIdx > 0)
				{
					CurrZone->PreviousColumnZone = Zones[RowIdx][ColumnIdx - 1];
				}
			}
		}
	}

	// Call initialized on the arena and all zones
	OnArenaInitialized();
	bArenaInitialized = true;
	for (TArray<AHorusArenaZone*>& CurrRow : Zones)
	{
		for (AHorusArenaZone* CurrZone : CurrRow)
		{
			CurrZone->OnArenaInitialized();
		}
	}

	return;
}

AHorusArenaZone* AHorusArena::GetArenaZone(int32 Column, int32 Row) const
{
	if (Column < NumColumns && Row < NumRows)
	{
		return Zones[Column][Row];
	}
	return nullptr;
}

#if WITH_EDITOR

void AHorusArena::UpdateZoneMappings()
{
	// Initialized variables for zone placement
	FVector BaseSpawnLoc = FVector(-ArenaHalfLength + (0.5f * RowLength), -ArenaHalfWidth + (0.5f * ColumnWidth), ZoneSpawnHeightOffset);
	FVector SpawnOffset = FVector::ZeroVector;

	TArray<AActor*> ArenaZones;
	UGameplayStatics::GetAllActorsOfClass(this, AHorusArenaZone::StaticClass(), ArenaZones);

	// Position and populate each row
	for (int32 RowIdx = 0; RowIdx < NumRows; RowIdx++)
	{
		SpawnOffset.X = ((float)RowIdx / NumRows) * ArenaHalfLength * 2.0f;

		// Position and populate each column
		for (int32 ColumnIdx = 0; ColumnIdx < NumColumns; ColumnIdx++)
		{
			SpawnOffset.Y = ((float)ColumnIdx / NumColumns) * ArenaHalfWidth * 2.0f;
			UHorusVisualBoxComponent* CurrVis = ZoneVisualizations[RowIdx][ColumnIdx];

			// Get the zone with the shortest distance to this visualization component
			TArray<AHorusArenaZone*> OverlappingZones;

			for (AActor* CurrActor : ArenaZones)
			{
				AHorusArenaZone* CurrZone = Cast<AHorusArenaZone>(CurrActor);
				FCollisionShape Shape = FCollisionShape::MakeBox(CurrZone->VisBox->GetScaledBoxExtent());
				if (CurrVis->OverlapComponent(CurrZone->VisBox->GetComponentLocation(), CurrZone->VisBox->GetComponentTransform().GetRotation(), Shape))
				{
					OverlappingZones.Add(CurrZone);
				}
			}
			//CurrVis->GetOverlappingActors(OverlappingZones, TSubclassOf<AHorusArenaZone>());
		
			float BestDistSquared = 999999999.0f;
			AHorusArenaZone* BestZone = nullptr;
			for (AHorusArenaZone* CurrZone : OverlappingZones)
			{
				float DistSquared = FVector::DistSquared(CurrVis->GetComponentLocation(), CurrZone->GetActorLocation());
				if (DistSquared < BestDistSquared)
				{
					// Check if the zone is already associated with a space
					if (CurrZone->Arena == this)
					{
						// Check if the distance to the old space is shorter than the distance to this space
						float OldDistSquared = FVector::DistSquared(ZoneVisualizations[CurrZone->RowIdx][CurrZone->ColumnIdx]->GetComponentLocation(), 
							CurrZone->GetActorLocation());
						if (OldDistSquared > DistSquared)
						{
							BestDistSquared = DistSquared;
							BestZone = CurrZone;
						}
					}
					else
					{
						BestDistSquared = DistSquared;
						BestZone = CurrZone;
					}
				}
			}

			// If an appropriate zone was found, update it accordingly
			if (BestZone)
			{
				BestZone->SetActorLocationAndRotation(BaseSpawnLoc + SpawnOffset + GetActorLocation(), GetActorRotation());
				BestZone->Arena = this;
				BestZone->RowIdx = RowIdx;
				BestZone->ColumnIdx = ColumnIdx;
			}
		}
	}


	return;
}

#endif

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
				UHorusVisualBoxComponent*& CurrVis = ZoneVisualizations[RowIdx][ColumnIdx];
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

