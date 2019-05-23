// ©Justin Camden 2019, all rights reserved..

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HorusArenaZone.h"
#include "HorusArenaZoneGenerator.generated.h"

class AHorusArenaGenZone;

UCLASS()
class HORUS_API AHorusArenaZoneGenerator : public AHorusArenaZone
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHorusArenaZoneGenerator(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called after generating a zone.
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = HorusArenaGenerator)
		void OnZoneGenerated();

	/** Returns the generated zone and destroys the spawner. */
	UFUNCTION(BlueprintCallable, Category = HorusArenaGenerator, meta = (UnsafeDuringActorConstruction))
		AHorusArenaZone* GenerateZone();


#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	/** Table for types of zones to spawn. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = HorusArenaZoneGenerator, meta = (AllowPrivateAccess = "true"))
		TMap <TSubclassOf<AHorusArenaGenZone>, int32> ZoneTypeSpawnChance;

};
