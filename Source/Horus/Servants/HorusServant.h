// ©Justin Camden 2019, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HorusServant.generated.h"

UCLASS()
class HORUS_API AHorusServant : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHorusServant();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
