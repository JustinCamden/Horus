// ©Justin Camden 2019, all rights reserved.

#pragma once

#include "Engine.h"
#include "ModuleManager.h"
#include "UnrealEd.h"

DECLARE_LOG_CATEGORY_EXTERN(HorusEditor, All, All)

class FHorusEditorModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

};