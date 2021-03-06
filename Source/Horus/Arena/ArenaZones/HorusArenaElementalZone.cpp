// ©Justin Camden 2019, all rights reserved.


#include "HorusArenaElementalZone.h"
#include "Core/HorusElement.h"

// Sets default values
AHorusArenaElementalZone::AHorusArenaElementalZone(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	Element = UHorusElement::StaticClass();

}

#if WITH_EDITOR
void AHorusArenaElementalZone::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	if (PropertyChangedEvent.Property != NULL && PropertyChangedEvent.Property->GetName() == GET_MEMBER_NAME_CHECKED(AHorusArenaElementalZone, Element).ToString())
	{
		if (Element == NULL)
		{
			Element = UHorusElement::StaticClass();
		}
	}

}
#endif

