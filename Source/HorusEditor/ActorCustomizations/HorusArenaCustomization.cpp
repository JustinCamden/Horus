// ©Justin Camden 2019, all rights reserved.

#include "HorusArenaCustomization.h"
#include "Engine.h"
#include "UnrealEd.h"
#include "PropertyEditing.h"
#include "IDetailsView.h"
#include "Horus/Arena/HorusArena.h"

#define LOCTEXT_NAMESPACE "DungeonArchitectEditorModule" 
DEFINE_LOG_CATEGORY(LogHorusArenaCustomization)

template<typename T>
T* GetBuilderObject(IDetailLayoutBuilder* DetailBuilder) {
	TArray<TWeakObjectPtr<UObject>> OutObjects;
	DetailBuilder->GetObjectsBeingCustomized(OutObjects);
	T* Obj = nullptr;
	if (OutObjects.Num() > 0) {
		Obj = Cast<T>(OutObjects[0].Get());
	}
	return Obj;
}

//////////////// FHorusArenaCustomization ////////////////////
void FHorusArenaCustomization::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
	IDetailCategoryBuilder& Category = DetailBuilder.EditCategory("HorusArena");
	FText Caption = FText::FromString("Update Zone Mappings");
	Category.AddCustomRow(Caption)
		.ValueContent()
		[
			SNew(SButton)
			.Text(Caption)
		.OnClicked(FOnClicked::CreateStatic(&FHorusArenaCustomization::ExecuteCommand, &DetailBuilder))
		];
}

TSharedRef<IDetailCustomization> FHorusArenaCustomization::MakeInstance()
{
	return MakeShareable(new FHorusArenaCustomization);
}

FReply FHorusArenaCustomization::ExecuteCommand(IDetailLayoutBuilder* DetailBuilder)
{
	USelection* SelectedActors = GEditor->GetSelectedActors();
	for (FSelectionIterator Iter(*SelectedActors); Iter; ++Iter)
	{
		AHorusArena* Arena = Cast<AHorusArena>(*Iter);
		if (Arena) {
			UE_LOG(LogHorusArenaCustomization, Log, TEXT("%s: Updating arena zone mappings."), *GetNameSafe(Arena));
			//Arena->UpdateZoneMappings();
		}
	}
	return FReply::Handled();
}

#undef LOCTEXT_NAMESPACE