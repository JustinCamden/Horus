// ©Justin Camden 2019, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "HorusVisBoxComponent.generated.h"

/**
 * Overridden class to expose visualizations on box components.
 */
UCLASS()
class HORUS_API UHorusVisBoxComponent : public UBoxComponent
{
	GENERATED_BODY()
public:
	/** Sets the line thickness of the box component. */
	UFUNCTION(BlueprintCallable, Category = HorusVisBoxComponent)
		void SetLineThickness(float NewLineThickness);
};
