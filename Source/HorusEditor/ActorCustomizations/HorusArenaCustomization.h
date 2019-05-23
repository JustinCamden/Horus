// ©Justin Camden 2019, all rights reserved.

#pragma once

#include "IDetailCustomization.h"
#include "IPropertyChangeListener.h"

DECLARE_LOG_CATEGORY_EXTERN(LogHorusArenaCustomization, Log, All);

class HORUSEDITOR_API FHorusArenaCustomization : public IDetailCustomization {
public:
	// IDetailCustomization interface
	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override;
	// End of IDetailCustomization interface

	static TSharedRef<IDetailCustomization> MakeInstance();

	static FReply ExecuteCommand(IDetailLayoutBuilder* DetailBuilder);
};