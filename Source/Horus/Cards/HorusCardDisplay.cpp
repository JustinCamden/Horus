// ©Justin Camden 2019, all rights reserved.

#include "HorusCardDisplay.h"
#include "Components/StaticMeshComponent.h"


FName AHorusCardDisplay::CardMeshComponentName(TEXT("CardMesh0"));

AHorusCardDisplay::AHorusCardDisplay(const FObjectInitializer& ObjectInitializer /*= FObjectInitializer::Get()*/)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;
	CardMesh = CreateDefaultSubobject<UStaticMeshComponent>(CardMeshComponentName);
	RootComponent = CardMesh;
}

// Called when the game starts or when spawned
void AHorusCardDisplay::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHorusCardDisplay::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

