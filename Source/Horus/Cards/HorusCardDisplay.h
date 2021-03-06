// ©Justin Camden 2019, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HorusCardDisplay.generated.h"

UCLASS()
class HORUS_API AHorusCardDisplay : public AActor
{
	GENERATED_BODY()
	
public:	
	/** Name of the MeshComponent. Use this name if you want to prevent creation of the component (with ObjectInitializer.DoNotCreateDefaultSubobject). */
	static FName CardMeshComponentName;

	// Sets default values for this actor's properties
	AHorusCardDisplay(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	/** Base mesh for displaying the card. */
	UPROPERTY(Category = HorusSpellCard, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* CardMesh;

};
