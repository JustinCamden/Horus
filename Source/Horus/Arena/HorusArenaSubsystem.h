// ©Justin Camden 2019, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "HorusArena.h"
#include "HorusArenaSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class HORUS_API UHorusArenaSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	/** Loads the arena database. */
	bool LoadArenaDatabase();


	/** Getter for the arena database. */
	UFUNCTION(BlueprintCallable, BlueprintPure)
		const TMap<FName, FHorusArenaData>& GetArenaDatabase() 
	{ 
#if WITH_EDITOR
		if (!bArenaDatabaseLoaded)
		{
			LoadArenaDatabase();
		}
#endif
		return ArenaDatabase;
	}



private:
	/** Loaded arena setups by name. */
	TMap <FName, FHorusArenaData> ArenaDatabase;

#if WITH_EDITORONLY_DATA
	/** Whether the arena database has been loaded.*/
	bool bArenaDatabaseLoaded;
#endif
	
};
