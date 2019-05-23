// ©Justin Camden 2019, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HorusArenaZone.generated.h"

class AHorusArena;

#if WITH_EDITOR
class UHorusVisualBoxComponent;
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

#if WITH_EDITOR
	/** Box component for visualization and overlap checks with the arena. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = HorusArenaZone, meta = (AllowPrivateAccess = "true"))
		UHorusVisualBoxComponent* VisBox;
#endif

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


private:
	/** Root scene component. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = HorusArenaZone, meta = (AllowPrivateAccess = "true"))
		USceneComponent* SceneRoot;

};
