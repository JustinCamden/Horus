// ©Justin Camden 2019, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "HorusEffect.generated.h"

/**
 * Base class for gameplay effects that can occur in the game.
 */
UCLASS()
class HORUS_API UHorusEffect : public UObject
{
	GENERATED_BODY()

public:
	/** Returns whether this effect should activate. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = HorusEffectBase)
		bool CanActivate(const TArray<AActor*>& ContextualActors);
	
	/** Implementation of the effect's execution. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = HorusEffectBase)
		void ActivateEffect(TArray<AActor*>& ContextualActors);
	
};

USTRUCT(BlueprintType)
struct FHorusEffectContext
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UHorusEffect* BaseEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<AActor*> ContextualActors;
};

