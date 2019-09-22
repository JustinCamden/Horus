// ©Justin Camden 2019, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Core/HorusUtilityLibrary.h"
#include "HorusArenaZone.generated.h"

class AHorusArena;

#if WITH_EDITORONLY_DATA

	class UHorusVisBoxComponent;

#endif

UCLASS()
class HORUS_API AHorusArenaZone : public AActor
{
	GENERATED_BODY()
	
public:	
	/** Name of the Scene Root Component. Use this name if you want to prevent creation of the component (with ObjectInitializer.DoNotCreateDefaultSubobject). */
	static FName SceneRootName;

	// Sets default values for this actor's properties
	AHorusArenaZone(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/** Reference to the owning arena. */
	UPROPERTY(BlueprintReadOnly, Category = HorusArenaZone)
		AHorusArena* Arena;

	/** Reference to the neighboring zone in the next row.*/
	UPROPERTY(BlueprintReadOnly, Category = HorusArenaZone)
		AHorusArenaZone* NextRowZone;

	/** Reference to the neighboring zone in the previous row.*/
	UPROPERTY(BlueprintReadOnly, Category = HorusArenaZone)
		AHorusArenaZone* PreviousRowZone;

	/** Reference to the neighboring zone in the next column.*/
	UPROPERTY(BlueprintReadOnly, Category = HorusArenaZone)
		AHorusArenaZone* NextColumnZone;

	/** Reference to the neighboring zone in the previous column.*/
	UPROPERTY(BlueprintReadOnly, Category = HorusArenaZone)
		AHorusArenaZone* PreviousColumnZone;

	/** Called when the arena is fully initialized. */
	UFUNCTION(BlueprintImplementableEvent, Category = HorusArenaZone)
		void OnArenaInitialized();

	/** Row Idx of this zone. */
	UPROPERTY(BlueprintReadOnly, Category = HorusArenaZone)
		int32 RowIdx;

	/** Column Idx of this zone. */
	UPROPERTY(BlueprintReadOnly, Category = HorusArenaZone)
		int32 ColumnIdx;

	/** Returns whether this zone is traversable. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = HorusArenaZone)
	bool IsTraversable() const;

#if WITH_EDITORONLY_DATA

	/** Box component for visualization and overlap checks with the arena. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = HorusArenaZone, meta = (AllowPrivateAccess = "true"))
		UHorusVisBoxComponent* VisBox;
#endif


	/** Gets a random Arena Zone class from a map of zone classes to relative probability. */
	UFUNCTION(BlueprintCallable, Category = HorusUtilityLibrary)
		static void GetRandomArenaZoneClassFromProbabilityPackage(const TMap<TSubclassOf<AHorusArenaZone>, uint8>& EntriesToRelativeProbability, TSubclassOf<AHorusArenaZone>& OutArenaZoneClass);


	/** Gets a soft pointer random Arena Zone class from a map of zone classes to relative probability. */
	UFUNCTION(BlueprintCallable, Category = HorusUtilityLibrary) static void GetRandomSoftArenaZoneClassFromProbabilityPackage(const TMap<TSoftClassPtr<AHorusArenaZone>, uint8>& EntriesToRelativeProbability, TSoftClassPtr<AHorusArenaZone>& OutArenaZoneClass);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


private:
	/** Root scene component. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = HorusArenaZone, meta = (AllowPrivateAccess = "true"))
		USceneComponent* SceneRoot;

};

FORCEINLINE void AHorusArenaZone::GetRandomArenaZoneClassFromProbabilityPackage(const TMap<TSubclassOf<AHorusArenaZone>, uint8>& EntriesToRelativeProbability, TSubclassOf<AHorusArenaZone>& OutArenaZoneClass)
{
	GetRandomEntryFromProbabilityPackage(EntriesToRelativeProbability, OutArenaZoneClass);
	return;
}

FORCEINLINE void AHorusArenaZone::GetRandomSoftArenaZoneClassFromProbabilityPackage(const TMap<TSoftClassPtr<AHorusArenaZone>, uint8>& EntriesToRelativeProbability, TSoftClassPtr<AHorusArenaZone>& OutArenaZoneClass)
{
	GetRandomEntryFromProbabilityPackage(EntriesToRelativeProbability, OutArenaZoneClass);
	return;
}