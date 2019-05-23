// ©Justin Camden 2019, all rights reserved.

#include "HorusEditor.h"
#include "ModuleManager.h"
#include "ActorCustomizations/HorusArenaCustomization.h"
#include "PropertyEditorModule.h"
#include "Engine.h"
#include "UnrealEd.h"

IMPLEMENT_GAME_MODULE(FHorusEditorModule, HorusEditor);

DEFINE_LOG_CATEGORY(HorusEditor)

#define LOCTEXT_NAMESPACE "HorusEditor"

void FHorusEditorModule::StartupModule()
{
	UE_LOG(HorusEditor, Warning, TEXT("HorusEditor: Log Started"));

	// Register the details customization
	FPropertyEditorModule& PropertyEditorModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	// Leave out the suffix "A" when specifying the name of your actor in the first param
	PropertyEditorModule.RegisterCustomClassLayout("HorusArena", FOnGetDetailCustomizationInstance::CreateStatic(&FHorusArenaCustomization::MakeInstance));
}

void FHorusEditorModule::ShutdownModule()
{
	UE_LOG(HorusEditor, Warning, TEXT("HorusEditor: Log Ended"));
}

#undef LOCTEXT_NAMESPACE