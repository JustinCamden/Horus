// ©Justin Camden 2019, all rights reserved.


#include "HorusArenaEditor.h"
#include "Runtime/Engine/Public/EngineUtils.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Horus/Arena/ArenaZones/HorusArenaZone.h"
#include "Horus/Core/Tools/HorusVisBoxComponent.h"
#include "HorusEditorFunctionLibrary.h"

// Sets default values
AHorusArenaEditor::AHorusArenaEditor(const FObjectInitializer& ObjectInitializer /*= FObjectInitializer::Get()*/)
	:Super(ObjectInitializer)
{

}

void AHorusArenaEditor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	ArenaData.DefaultZoneClass = DefaultZoneClass;
	ArenaData.Rows.SetNum(NumRows);
	for (FHorusArenaRowData& CurrRow : ArenaData.Rows)
	{
		CurrRow.Columns.SetNum(NumColumns);
	}

}

void AHorusArenaEditor::MapWorldZones()
{
	// Clear arena data
	for (FHorusArenaRowData& CurrRow : ArenaData.Rows)
	{
		for (auto& CurrZoneClass : CurrRow.Columns)
		{
			CurrZoneClass = nullptr;
		}
	}
	for (FHorusArenaRow& CurrRow : Rows)
	{
		for (auto& CurrZone : CurrRow.Columns)
		{
			CurrZone->PreviousColumnZone = nullptr;
			CurrZone->NextColumnZone = nullptr;
			CurrZone->PreviousRowZone = nullptr;
			CurrZone->NextRowZone = nullptr;
			CurrZone = nullptr;
		}
	}

	// Disinherit current mapped zones
	FDetachmentTransformRules DetachmentRules = FDetachmentTransformRules(EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, true);
	for (AHorusArenaZone* CurrZone : MappedZones)
	{
		if (CurrZone)
		{
			CurrZone->Arena = nullptr;
			CurrZone->DetachFromActor(DetachmentRules);
		}
	}
	MappedZones.Empty();

	// Get a reference to each arena zone
	TArray<AHorusArenaZone*> ArenaZones;
	UWorld* World = GetWorld();
	{
		for (TActorIterator<AHorusArenaZone> It(World, AHorusArenaZone::StaticClass()); It; ++It)
		{
			AHorusArenaZone* ArenaZone = *It;
			if (!ArenaZone->IsPendingKill())
			{
				ArenaZones.Add(ArenaZone);
			}
		}
	}

	// Populate each row
	for (int32 RowIdx = 0; RowIdx < NumRows; RowIdx++)
	{
		// Populate each column
		for (int32 ColumnIdx = 0; ColumnIdx < NumColumns; ColumnIdx++)
		{
			GetBestMappingForZone(RowIdx, ColumnIdx, ArenaZones);
		}
	}

	// Update mappings for arena data
	FAttachmentTransformRules AttachmentRules = FAttachmentTransformRules(EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, false);
	for (AHorusArenaZone* CurrZone : MappedZones)
	{
		ArenaData.Rows[CurrZone->RowIdx].Columns[CurrZone->ColumnIdx] = CurrZone->GetClass();
		Rows[CurrZone->RowIdx].Columns[CurrZone->ColumnIdx] = CurrZone;
		CurrZone->AttachToActor(this, AttachmentRules);
	}
	MapZoneNeighbors();

	return;
}

void AHorusArenaEditor::GetBestMappingForZone(int32 RowIdx, int32 ColumnIdx, TArray<AHorusArenaZone*>& ArenaZones)
{
	// Initialize spawn parameters
	UHorusVisBoxComponent* CurrVis = ZoneVisualizations[RowIdx][ColumnIdx];

	// Get all the zones overlapping this vis box
	TArray<AHorusArenaZone*> OverlappingZones;
	for (AHorusArenaZone* CurrZone: ArenaZones)
	{
		FCollisionShape Shape = FCollisionShape::MakeBox(CurrZone->VisBox->GetScaledBoxExtent());
		if (CurrVis->OverlapComponent(CurrZone->VisBox->GetComponentLocation(), CurrZone->VisBox->GetComponentTransform().GetRotation(), Shape))
		{
			OverlappingZones.Add(CurrZone);
		}
	}

	// Early exit if there are no arena zones overlapping
	if (OverlappingZones.Num() <= 0)
	{
		return;
	}

	// Of the zones overlapping this vis box, find the one with the shortest distance
	bool bStoleBestZone = false;
	float BestDistSquared = BIG_NUMBER;
	AHorusArenaZone* BestZone = nullptr;
	for (AHorusArenaZone* CurrZone : OverlappingZones)
	{
		float DistSquared = FVector::DistSquared(CurrVis->GetComponentLocation(), CurrZone->GetActorLocation());
		if (DistSquared < BestDistSquared)
		{
			// Check if the zone is already associated with a space
			if (MappedZones.Contains(CurrZone) && (CurrZone->RowIdx != RowIdx || CurrZone->ColumnIdx != ColumnIdx))
			{
				// Check if the distance to the old space is shorter than the distance to this space
				float OldDistSquared = FVector::DistSquared(ZoneVisualizations[CurrZone->RowIdx][CurrZone->ColumnIdx]->GetComponentLocation(),
					CurrZone->GetActorLocation());
				if (OldDistSquared > DistSquared)
				{
					// If the distance to this space is less than the distance to the old space, 
					// we should steal this zone, provided it is the best for us
					BestDistSquared = DistSquared;
					BestZone = CurrZone;
					bStoleBestZone = true;
				}
			}

			// If not, we don't need to still it
			else
			{
				BestDistSquared = DistSquared;
				BestZone = CurrZone;
				bStoleBestZone = false;
			}
		}
	}

	// If an appropriate zone was found, update it accordingly
	if (BestZone)
	{
		MappedZones.Add(BestZone);

		// Instruct zone's old mapping to check again if appropriate
		if (bStoleBestZone)
		{
			int32 OldZoneRowIdx = BestZone->RowIdx;
			int32 OldZoneColumnIdx = BestZone->ColumnIdx;
			BestZone->Arena = this;
			BestZone->RowIdx = RowIdx;
			BestZone->ColumnIdx = ColumnIdx;
			GetBestMappingForZone(OldZoneRowIdx, OldZoneColumnIdx, ArenaZones);
		}
		BestZone->Arena = this;
		BestZone->RowIdx = RowIdx;
		BestZone->ColumnIdx = ColumnIdx;
	}

	return;
}

void AHorusArenaEditor::PositionZones()
{
	// Initialize variables for zone placement
	FVector BaseSpawnLoc = FVector(-ArenaHalfLength + (0.5f * RowLength), (-ArenaHalfWidth + (0.5f * ColumnWidth)), ZoneSpawnHeightOffset);

	// Position each zone
	FString ContextString = "Positioning Arena Zones for Arena ";
	for (AHorusArenaZone* CurrZone : MappedZones)
	{
		if (CurrZone)
		{
			FVector SpawnOffset = FVector((((float)CurrZone->RowIdx / NumRows) * ArenaHalfLength * 2.0f), (((float)CurrZone->ColumnIdx / NumColumns) * ArenaHalfWidth * 2.0f), 0);
			CurrZone->SetActorRelativeLocation(BaseSpawnLoc + SpawnOffset);
			CurrZone->SetActorRelativeRotation(FRotator());
		}
		else
		{
			MappedZones.Remove(CurrZone);
		}
	}

	return;
}

void AHorusArenaEditor::ImportArenaData()
{
	// Check if data table is valid
	if (!ArenaDataTable)
	{
		UE_LOG(LogHorusArena, Error, TEXT("%s ERROR attempting to load data from data table: Arena Data Table was not valid!."), *GetNameSafe(this));
		return;
	}

	// Check if the data table is the right type
	if (ArenaDataTable->GetRowStructName().ToString() != "HorusArenaDataTableRow")
	{
		UE_LOG(LogHorusArena, Error, TEXT("%s ERROR attempting to load data from data table: Data Table row struct was not HorusArenaDataTableRow! Was %s."), *GetNameSafe(this), *ArenaDataTable->GetRowStructName().ToString());
		return;
	}

	// Get the data from the data table
	FString ContextString = GetNameSafe(this);
	ContextString += ": Attempting to load arena data from data table ";
	ContextString += GetNameSafe(ArenaDataTable);
	FHorusArenaDataTableRow* DataTableRow = ArenaDataTable->FindRow<FHorusArenaDataTableRow>(ArenaName, ContextString, true);
	if (!DataTableRow)
	{
		UE_LOG(LogHorusArena, Error, TEXT("%s ERROR attempting to load data from data table %s: Could not find entry for Arena Name %."), *GetNameSafe(this), *GetNameSafe(ArenaDataTable), *ArenaName.ToString());
		return;
	}

	// If we found the data, then update our own data
	ArenaData.DefaultZoneClass = DataTableRow->DefaultZoneClass;
	ArenaData.Rows = DataTableRow->Rows;
	NumRows = Rows.Num();
	if (NumRows > 0)
	{
		NumColumns = Rows[0].Columns.Num();
	}

	return;

}

void AHorusArenaEditor::ExportArenaData()
{
	// Check if data table is valid
	if (!ArenaDataTable)
	{
		UE_LOG(LogHorusArena, Error, TEXT("%s ERROR attempting to save data to data table: Arena Data Table was not valid!."), *GetNameSafe(this));
		return;
	}

	// Check if the data table is the right type
	if (ArenaDataTable->GetRowStructName().ToString() != "HorusArenaDataTableRow")
	{
		UE_LOG(LogHorusArena, Error, TEXT("%s ERROR attempting to save data to data table: Data Table row struct was not HorusArenaDataTableRow! Was %s."), *GetNameSafe(this), *ArenaDataTable->GetRowStructName().ToString());
		return;
	}

	// Find the data in the data table
	FString ContextString = GetNameSafe(this);
	ContextString += ": Attempting to save arena data to data table ";
	ContextString += GetNameSafe(ArenaDataTable);
	FHorusArenaDataTableRow* DataTableRow = ArenaDataTable->FindRow<FHorusArenaDataTableRow>(ArenaName, ContextString, true);
	if (!DataTableRow)
	{
		FHorusArenaDataTableRow NewRow;
		NewRow.DefaultZoneClass = ArenaData.DefaultZoneClass;
		NewRow.Rows = ArenaData.Rows;
		ArenaDataTable->AddRow(ArenaName, NewRow);

		return;
	}

	// If we found the data, then update the data to match our own data
	DataTableRow->DefaultZoneClass = ArenaData.DefaultZoneClass;
	DataTableRow->Rows = ArenaData.Rows;

	return;
}

void AHorusArenaEditor::Editor_SpawnZones()
{
	// Clear arena data
	for (FHorusArenaRow& CurrRow : Rows)
	{
		for (auto& CurrZone : CurrRow.Columns)
		{
			CurrZone->PreviousColumnZone = nullptr;
			CurrZone->NextColumnZone = nullptr;
			CurrZone->PreviousRowZone = nullptr;
			CurrZone->NextRowZone = nullptr;
			CurrZone = nullptr;
		}
	}

	// Disinherit current mapped zones
	FDetachmentTransformRules DetachmentRules = FDetachmentTransformRules(EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, true);
	for (AHorusArenaZone* CurrZone : MappedZones)
	{
		if (CurrZone)
		{
			CurrZone->Arena = nullptr;
			CurrZone->DetachFromActor(DetachmentRules);
		}
	}

	MappedZones.Empty();

	SpawnZones();

	// Add each zone to mapped zones
	for (FHorusArenaRow& CurrRow : Rows)
	{
		for (AHorusArenaZone* CurrZone : CurrRow.Columns)
		{
			MappedZones.Add(CurrZone);
		}
	}

	return;
}

void AHorusArenaEditor::DeleteChildZones()
{
	// Disinherit current mapped zones
	for (FHorusArenaRow& CurrRow : Rows)
	{
		for (auto& CurrZone : CurrRow.Columns)
		{
			CurrZone = nullptr;
		}
	}

	// Destroy mapped zones
	for (AHorusArenaZone* CurrZone : MappedZones)
	{
		if (CurrZone)
		{
			CurrZone->Destroy();
		}
	}
	MappedZones.Empty();

	return;
}

void AHorusArenaEditor::DeleteOrphanZones()
{
	// Get a reference to each arena zone
	TArray<AHorusArenaZone*> ArenaZones;
	UWorld* World = GetWorld();
	{
		for (TActorIterator<AHorusArenaZone> It(World, AHorusArenaZone::StaticClass()); It; ++It)
		{
			AHorusArenaZone* Actor = *It;
			if (!Actor->IsPendingKill())
			{
				ArenaZones.Add(Actor);
			}
		}
	}

	// Delete any zone that is not currently mapped
	for (AHorusArenaZone* CurrZone : ArenaZones)
	{
		if (!MappedZones.Contains(CurrZone))
		{
			CurrZone->Destroy();
		}
	}

	return;
}

void AHorusArenaEditor::DeleteAllZones()
{
	// Clear arena data
	for (FHorusArenaRow& CurrRow : Rows)
	{
		for (auto& CurrZone : CurrRow.Columns)
		{
			CurrZone = nullptr;
		}
	}
	MappedZones.Empty();

	// Destroy every zone
	UWorld* World = GetWorld();
	{
		for (TActorIterator<AHorusArenaZone> It(World, AHorusArenaZone::StaticClass()); It; ++It)
		{
			AHorusArenaZone* CurrZone = *It;
			CurrZone->Destroy();
		}
	}

	return;
}

void AHorusArenaEditor::ClearArenaData()
{
	for (FHorusArenaRowData& CurrRow : ArenaData.Rows)
	{
		for (auto& CurrZoneClass : CurrRow.Columns)
		{
			CurrZoneClass = nullptr;
		}
	}
	DefaultZoneClass = AHorusArenaZone::StaticClass();

	return;
}

void AHorusArenaEditor::DetachChildZones()
{
	// Disinherit current mapped zones
	for (FHorusArenaRow& CurrRow : Rows)
	{
		for (auto& CurrZone : CurrRow.Columns)
		{
			CurrZone = nullptr;
		}
	}
	FDetachmentTransformRules DetachmentRules = FDetachmentTransformRules(EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, true);
	for (AHorusArenaZone* CurrZone : MappedZones)
	{
		if (CurrZone)
		{
			CurrZone->Arena = nullptr;
			CurrZone->DetachFromActor(DetachmentRules);
		}
	}
	MappedZones.Empty();

	return;
}
