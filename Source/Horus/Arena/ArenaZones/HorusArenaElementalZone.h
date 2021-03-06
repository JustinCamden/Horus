// ©Justin Camden 2019, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HorusArenaZone.h"
#include "HorusArenaElementalZone.generated.h"

class UHorusElement;

UCLASS()
class HORUS_API AHorusArenaElementalZone : public AHorusArenaZone
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHorusArenaElementalZone(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/** Element to associate this zone with. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HorusArenaZone|ElementalZone")
		TSubclassOf<UHorusElement> Element;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

};
