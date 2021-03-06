// ©Justin Camden 2019, all rights reserved..

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HorusArenaZone.h"
#include "HorusArenaZoneGenerator.generated.h"


UCLASS()
class HORUS_API AHorusArenaZoneGenerator : public AHorusArenaZone
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHorusArenaZoneGenerator(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// Called after generating a zone.
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = HorusArenaZoneGenerator)
		void OnZoneGenerated();

	/** Returns the generated zone and destroys the spawner. */
	UFUNCTION(BlueprintCallable, Category = HorusArenaZoneGenerator, meta = (UnsafeDuringActorConstruction))
		AHorusArenaZone* GenerateZone();

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif


private:

	/** Table for types of zones to spawn. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = HorusArenaZoneGenerator, meta = (AllowPrivateAccess = "true"))
		TMap <TSubclassOf<AHorusArenaZone>, int32> ZoneTypeSpawnChance;

};
