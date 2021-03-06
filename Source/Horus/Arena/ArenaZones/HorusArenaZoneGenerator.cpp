// ©Justin Camden 2019, all rights reserved..


#include "HorusArenaZoneGenerator.h"
#include "Arena/HorusArena.h"


// Sets default values
AHorusArenaZoneGenerator::AHorusArenaZoneGenerator(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	// Initialize variables
	ZoneTypeSpawnChance.Add(AHorusArenaZone::StaticClass(), 1);

}

AHorusArenaZone* AHorusArenaZoneGenerator::GenerateZone()
{
	// Create the random chance table
	TArray<TSubclassOf<AHorusArenaZone>> RandomChanceTable;
	int32 TargetIdx = 0;
	int32 BeginIdx = 0;

	// Add a number of entries to the table depending on the corresponding chance value
	for (auto& CurrZone : ZoneTypeSpawnChance)
	{
		TargetIdx += CurrZone.Value;
		for (int32 Idx = BeginIdx; Idx < TargetIdx; Idx++)
		{
			RandomChanceTable.Add(CurrZone.Key);
		}
		BeginIdx = TargetIdx;
	}

	// Use a random number to pick an entry from the table
	int32 Choice = FMath::RandRange(0, RandomChanceTable.Num() - 1);

	// Spawn the zone
	FTransform SpawnTransform = GetActorTransform();
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	AHorusArenaZone* RetVal = GetWorld()->SpawnActor<AHorusArenaZone>(RandomChanceTable[Choice], SpawnTransform, SpawnParameters);

	// Update zone references
	RetVal->Arena = Arena;
	RetVal->RowIdx = RowIdx;
	RetVal->ColumnIdx = ColumnIdx;
	RetVal->NextRowZone = NextRowZone;
	RetVal->PreviousColumnZone = PreviousColumnZone;
	RetVal->NextColumnZone = NextColumnZone;
	RetVal->PreviousRowZone = PreviousRowZone;
	if (Arena && Arena->IsArenaInitialized())
	{
		RetVal->OnArenaInitialized();
	}

	return RetVal;
}

#if WITH_EDITOR
void AHorusArenaZoneGenerator::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	if (PropertyChangedEvent.Property != NULL && PropertyChangedEvent.Property->GetName() == GET_MEMBER_NAME_CHECKED(AHorusArenaZoneGenerator, ZoneTypeSpawnChance).ToString())
	{
		// Remove all null reference keys
		if (ZoneTypeSpawnChance.Find(NULL))
		{
			int32 SpawnChance = ZoneTypeSpawnChance[NULL];
			ZoneTypeSpawnChance.Remove(NULL);
			if (!ZoneTypeSpawnChance.Find(AHorusArenaZone::StaticClass()))
			{
				ZoneTypeSpawnChance.Add(AHorusArenaZone::StaticClass(), SpawnChance);
			}
		}

		// Make sure there is always at least one entry in the table
		if (ZoneTypeSpawnChance.Num() <= 0)
		{
			ZoneTypeSpawnChance.Add(AHorusArenaZone::StaticClass(), 1);
		}

		// Make sure each entry's chance is valid
		for (auto& CurrEntry : ZoneTypeSpawnChance)
		{
			int32 SpawnChance = CurrEntry.Value;
			CurrEntry.Value = FMath::Max(SpawnChance, 1);
		}
	}

}
#endif
