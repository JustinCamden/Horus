// ©Justin Camden 2019, all rights reserved.

#include "HorusArenaZone.h"

#if WITH_EDITOR

#include "Core/Tools/HorusVisBoxComponent.h"

#endif

FName AHorusArenaZone::SceneRootName(TEXT("SceneRoot"));

// Sets default values
AHorusArenaZone::AHorusArenaZone(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;
	RowIdx = 0;
	ColumnIdx = 0;

	// Initialize components
	SceneRoot = CreateDefaultSubobject<USceneComponent>(SceneRootName);
	RootComponent = SceneRoot;

#if WITH_EDITOR
	VisBox = CreateDefaultSubobject<UHorusVisBoxComponent>(TEXT("VisBox"));
	if (VisBox)
	{
		VisBox->SetupAttachment(RootComponent);
		VisBox->SetBoxExtent(FVector(50.0f, 37.5f, 25.0f));
		VisBox->SetRelativeLocation(FVector(0.0f, 0.0f, 25.0f));
		VisBox->SetLineThickness(2.0f);
	}
#endif

}

// Called when the game starts or when spawned
void AHorusArenaZone::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AHorusArenaZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool AHorusArenaZone::IsTraversable_Implementation() const
{
	return true;
}
