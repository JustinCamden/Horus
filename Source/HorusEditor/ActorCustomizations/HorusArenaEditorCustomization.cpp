// ©Justin Camden 2019, all rights reserved.

#include "HorusArenaEditorCustomization.h"
#include "Engine.h"
#include "UnrealEd.h"
#include "PropertyEditing.h"
#include "IDetailsView.h"
#include "Horus/Arena/ArenaZones/HorusArenaZone.h"
#include "Editors/HorusArenaEditor.h"
#include "Developer/SourceControl/Public/SourceControlHelpers.h"

DEFINE_LOG_CATEGORY(LogHorusArena)

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

void FHorusArenaEditorCustomization::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
	IDetailCategoryBuilder& Category = DetailBuilder.EditCategory("HorusArenaEditor");
	FText Caption = FText::FromString("Map World Zones");
	Category.AddCustomRow(Caption)
		.ValueContent()
		[
			SNew(SButton)
			.Text(Caption)
		.OnClicked(FOnClicked::CreateStatic(&FHorusArenaEditorCustomization::MapWorldZones, &DetailBuilder))
		];
	Caption = FText::FromString("Position Zones");
	Category.AddCustomRow(Caption)
		.ValueContent()
		[
			SNew(SButton)
			.Text(Caption)
		.OnClicked(FOnClicked::CreateStatic(&FHorusArenaEditorCustomization::PositionZones, &DetailBuilder))
		];
	Caption = FText::FromString("Import Arena Data");
	Category.AddCustomRow(Caption)
		.ValueContent()
		[
			SNew(SButton)
			.Text(Caption)
		.OnClicked(FOnClicked::CreateStatic(&FHorusArenaEditorCustomization::ImportArenaData, &DetailBuilder))
		];
	Caption = FText::FromString("Export Arena Data");
	Category.AddCustomRow(Caption)
		.ValueContent()
		[
			SNew(SButton)
			.Text(Caption)
		.OnClicked(FOnClicked::CreateStatic(&FHorusArenaEditorCustomization::ExportArenaData, &DetailBuilder))
		];
	Caption = FText::FromString("Spawn Zones");
	Category.AddCustomRow(Caption)
		.ValueContent()
		[
			SNew(SButton)
			.Text(Caption)
		.OnClicked(FOnClicked::CreateStatic(&FHorusArenaEditorCustomization::SpawnZones, &DetailBuilder))
		];
	Caption = FText::FromString("Delete Child Zones");
	Category.AddCustomRow(Caption)
		.ValueContent()
		[
			SNew(SButton)
			.Text(Caption)
		.OnClicked(FOnClicked::CreateStatic(&FHorusArenaEditorCustomization::DeleteChildZones, &DetailBuilder))
		];
	Caption = FText::FromString("Delete Orphan Zones");
	Category.AddCustomRow(Caption)
		.ValueContent()
		[
			SNew(SButton)
			.Text(Caption)
		.OnClicked(FOnClicked::CreateStatic(&FHorusArenaEditorCustomization::DeleteOrphanZones, &DetailBuilder))
		];
	Caption = FText::FromString("Delete All Zones");
	Category.AddCustomRow(Caption)
		.ValueContent()
		[
			SNew(SButton)
			.Text(Caption)
		.OnClicked(FOnClicked::CreateStatic(&FHorusArenaEditorCustomization::DeleteAllZones, &DetailBuilder))
		];
	Caption = FText::FromString("Clear Arena Data");
	Category.AddCustomRow(Caption)
		.ValueContent()
		[
			SNew(SButton)
			.Text(Caption)
		.OnClicked(FOnClicked::CreateStatic(&FHorusArenaEditorCustomization::ClearArenaData, &DetailBuilder))
		];
	Caption = FText::FromString("Detach Child Zones");
	Category.AddCustomRow(Caption)
		.ValueContent()
		[
			SNew(SButton)
			.Text(Caption)
		.OnClicked(FOnClicked::CreateStatic(&FHorusArenaEditorCustomization::DetachChildZones, &DetailBuilder))
		];
}

TSharedRef<IDetailCustomization> FHorusArenaEditorCustomization::MakeInstance()
{
	return MakeShareable(new FHorusArenaEditorCustomization);
}

FReply FHorusArenaEditorCustomization::MapWorldZones(IDetailLayoutBuilder* DetailBuilder)
{
	USelection* SelectedActors = GEditor->GetSelectedActors();
	for (FSelectionIterator Iter(*SelectedActors); Iter; ++Iter)
	{
		AHorusArenaEditor* Arena = Cast<AHorusArenaEditor>(*Iter);
		if (Arena) 
		{
			FString Context = GetNameSafe(Arena);
			Context += ": Loading Zone Mappings from World Arena Zones";
			UE_LOG(LogHorusArena, Log, TEXT("%s"), *Context);
			UKismetSystemLibrary::BeginTransaction(Context, FText::FromString(Context), Arena);
			UKismetSystemLibrary::TransactObject(Arena);
			for (TActorIterator<AHorusArenaZone> It(Arena->GetWorld(), AHorusArenaZone::StaticClass()); It; ++It)
			{
				AHorusArenaZone* ArenaZone = *It;
				if (!ArenaZone->IsPendingKill())
				{
					UKismetSystemLibrary::TransactObject(ArenaZone);
				}
			}
			Arena->MapWorldZones();
			UKismetSystemLibrary::EndTransaction();
		}
	}
	return FReply::Handled();
}

FReply FHorusArenaEditorCustomization::PositionZones(IDetailLayoutBuilder* DetailBuilder)
{
	USelection* SelectedActors = GEditor->GetSelectedActors();
	for (FSelectionIterator Iter(*SelectedActors); Iter; ++Iter)
	{
		AHorusArenaEditor* Arena = Cast<AHorusArenaEditor>(*Iter);
		if (Arena) 
		{
			FString Context = GetNameSafe(Arena);
			Context += ": Positioning associated Arena Zones at spawn locations";
			UE_LOG(LogHorusArena, Log, TEXT("%s"), *Context);
			UKismetSystemLibrary::BeginTransaction(Context, FText::FromString(Context), Arena);
			UKismetSystemLibrary::TransactObject(Arena);
			for (TActorIterator<AHorusArenaZone> It(Arena->GetWorld(), AHorusArenaZone::StaticClass()); It; ++It)
			{
				AHorusArenaZone* ArenaZone = *It;
				if (!ArenaZone->IsPendingKill())
				{
					UKismetSystemLibrary::TransactObject(ArenaZone);
				}
			}
			Arena->PositionZones();
			UKismetSystemLibrary::EndTransaction();
		}
	}
	return FReply::Handled();
}

FReply FHorusArenaEditorCustomization::ImportArenaData(IDetailLayoutBuilder* DetailBuilder)
{
	USelection* SelectedActors = GEditor->GetSelectedActors();
	for (FSelectionIterator Iter(*SelectedActors); Iter; ++Iter)
	{
		AHorusArenaEditor* Arena = Cast<AHorusArenaEditor>(*Iter);
		if (Arena) 
		{
			FString Context = GetNameSafe(Arena);
			Context += ": Importing Arena Data from Data Table ";
			Context += GetNameSafe(Arena->ArenaDataTable);
			UE_LOG(LogHorusArena, Log, TEXT("%s"), *Context);
			UKismetSystemLibrary::BeginTransaction(Context, FText::FromString(Context), Arena);
			UKismetSystemLibrary::TransactObject(Arena);
			Arena->ImportArenaData();
			UKismetSystemLibrary::EndTransaction();
		}
	}
	return FReply::Handled();
}

FReply FHorusArenaEditorCustomization::ExportArenaData(IDetailLayoutBuilder* DetailBuilder)
{
	USelection* SelectedActors = GEditor->GetSelectedActors();
	for (FSelectionIterator Iter(*SelectedActors); Iter; ++Iter)
	{
		AHorusArenaEditor* Arena = Cast<AHorusArenaEditor>(*Iter);
		if (Arena) 
		{
			UDataTable* ArenaDataTable = Arena->ArenaDataTable;
			if (ArenaDataTable)
			{
				FString Context = GetNameSafe(Arena);
				Context += ": Exporting Arena Data to Data Table ";
				Context += GetNameSafe(ArenaDataTable);
				UE_LOG(LogHorusArena, Log, TEXT("%s"), *Context);
				UKismetSystemLibrary::BeginTransaction(Context, FText::FromString(Context), Arena->ArenaDataTable);
				UKismetSystemLibrary::TransactObject(ArenaDataTable);
				USourceControlHelpers::CheckOutOrAddFile(UKismetSystemLibrary::GetPathName(ArenaDataTable));
				Arena->ExportArenaData();
				UKismetSystemLibrary::EndTransaction();
			}
		}
	}
	return FReply::Handled();
}

FReply FHorusArenaEditorCustomization::SpawnZones(IDetailLayoutBuilder* DetailBuilder)
{
	USelection* SelectedActors = GEditor->GetSelectedActors();
	for (FSelectionIterator Iter(*SelectedActors); Iter; ++Iter)
	{
		AHorusArenaEditor* Arena = Cast<AHorusArenaEditor>(*Iter);
		if (Arena) 
		{
			FString Context = GetNameSafe(Arena);
			Context += ": Spawning Zones according to Arena Data";
			UE_LOG(LogHorusArena, Log, TEXT("%s"), *Context);
			UKismetSystemLibrary::BeginTransaction(Context, FText::FromString(Context), Arena->GetWorld());
			UKismetSystemLibrary::TransactObject(Arena->GetWorld());
			UKismetSystemLibrary::TransactObject(Arena);
			Arena->Editor_SpawnZones();
			UKismetSystemLibrary::EndTransaction();
		}
	}
	return FReply::Handled();
}

FReply FHorusArenaEditorCustomization::DeleteChildZones(IDetailLayoutBuilder* DetailBuilder)
{
	USelection* SelectedActors = GEditor->GetSelectedActors();
	for (FSelectionIterator Iter(*SelectedActors); Iter; ++Iter)
	{
		AHorusArenaEditor* Arena = Cast<AHorusArenaEditor>(*Iter);
		if (Arena) 
		{
			FString Context = GetNameSafe(Arena);
			Context += ": Deleting Child Zones";
			UE_LOG(LogHorusArena, Log, TEXT("%s"), *Context);
			UKismetSystemLibrary::BeginTransaction(Context, FText::FromString(Context), Arena->GetWorld());
			UKismetSystemLibrary::TransactObject(Arena->GetWorld());
			UKismetSystemLibrary::TransactObject(Arena);
			Arena->DeleteChildZones();
			UKismetSystemLibrary::EndTransaction();
		}
	}
	return FReply::Handled();
}

FReply FHorusArenaEditorCustomization::DeleteOrphanZones(IDetailLayoutBuilder* DetailBuilder)
{
	USelection* SelectedActors = GEditor->GetSelectedActors();
	for (FSelectionIterator Iter(*SelectedActors); Iter; ++Iter)
	{
		AHorusArenaEditor* Arena = Cast<AHorusArenaEditor>(*Iter);
		if (Arena) 
		{
			FString Context = GetNameSafe(Arena);
			Context += ": Deleting Orphan Zones";
			UE_LOG(LogHorusArena, Log, TEXT("%s"), *Context);
			UKismetSystemLibrary::BeginTransaction(Context, FText::FromString(Context), Arena->GetWorld());
			UKismetSystemLibrary::TransactObject(Arena->GetWorld());
			Arena->DeleteOrphanZones();
			UKismetSystemLibrary::EndTransaction();
		}
	}
	return FReply::Handled();
}

FReply FHorusArenaEditorCustomization::DeleteAllZones(IDetailLayoutBuilder* DetailBuilder)
{
	USelection* SelectedActors = GEditor->GetSelectedActors();
	for (FSelectionIterator Iter(*SelectedActors); Iter; ++Iter)
	{
		AHorusArenaEditor* Arena = Cast<AHorusArenaEditor>(*Iter);
		if (Arena) 
		{
			FString Context = GetNameSafe(Arena);
			Context += ": Deleting all Zones";
			UE_LOG(LogHorusArena, Log, TEXT("%s"), *Context);
			UKismetSystemLibrary::BeginTransaction(Context, FText::FromString(Context), Arena->GetWorld());
			UKismetSystemLibrary::TransactObject(Arena->GetWorld());
			UKismetSystemLibrary::TransactObject(Arena);
			Arena->DeleteAllZones();
			UKismetSystemLibrary::EndTransaction();
		}
	}
	return FReply::Handled();
}

FReply FHorusArenaEditorCustomization::ClearArenaData(IDetailLayoutBuilder* DetailBuilder)
{
	USelection* SelectedActors = GEditor->GetSelectedActors();
	for (FSelectionIterator Iter(*SelectedActors); Iter; ++Iter)
	{
		AHorusArenaEditor* Arena = Cast<AHorusArenaEditor>(*Iter);
		if (Arena) 
		{
			FString Context = GetNameSafe(Arena);
			Context += ": Clearing Arena Data";
			UE_LOG(LogHorusArena, Log, TEXT("%s"), *Context);
			UKismetSystemLibrary::BeginTransaction(Context, FText::FromString(Context), Arena);
			UKismetSystemLibrary::TransactObject(Arena);
			Arena->ClearArenaData();
			UKismetSystemLibrary::EndTransaction();
		}
	}
	return FReply::Handled();
}

FReply FHorusArenaEditorCustomization::DetachChildZones(IDetailLayoutBuilder* DetailBuilder)
{
	USelection* SelectedActors = GEditor->GetSelectedActors();
	for (FSelectionIterator Iter(*SelectedActors); Iter; ++Iter)
	{
		AHorusArenaEditor* Arena = Cast<AHorusArenaEditor>(*Iter);
		if (Arena) 
		{
			FString Context = GetNameSafe(Arena);
			Context += ": Detaching Child Zones";
			UE_LOG(LogHorusArena, Log, TEXT("%s"), *Context);
			UKismetSystemLibrary::BeginTransaction(Context, FText::FromString(Context), Arena);
			UKismetSystemLibrary::TransactObject(Arena);
			for (TActorIterator<AHorusArenaZone> It(Arena->GetWorld(), AHorusArenaZone::StaticClass()); It; ++It)
			{
				AHorusArenaZone* ArenaZone = *It;
				if (!ArenaZone->IsPendingKill())
				{
					UKismetSystemLibrary::TransactObject(ArenaZone);
				}
			}
			Arena->DetachChildZones();
			UKismetSystemLibrary::EndTransaction();
		}
	}
	return FReply::Handled();
}
