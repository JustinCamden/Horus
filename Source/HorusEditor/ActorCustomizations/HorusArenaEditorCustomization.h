// ©Justin Camden 2019, all rights reserved.

#pragma once

#include "IDetailCustomization.h"
#include "IPropertyChangeListener.h"

class HORUSEDITOR_API FHorusArenaEditorCustomization : public IDetailCustomization {
public:
	// IDetailCustomization interface
	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override;
	// End of IDetailCustomization interface

	static TSharedRef<IDetailCustomization> MakeInstance();

	static FReply MapWorldZones(IDetailLayoutBuilder* DetailBuilder);
	static FReply PositionZones(IDetailLayoutBuilder* DetailBuilder);
	static FReply ImportArenaData(IDetailLayoutBuilder* DetailBuilder);
	static FReply ExportArenaData(IDetailLayoutBuilder* DetailBuilder);
	static FReply SpawnZones(IDetailLayoutBuilder* DetailBuilder);
	static FReply DeleteChildZones(IDetailLayoutBuilder* DetailBuilder);
	static FReply DeleteOrphanZones(IDetailLayoutBuilder* DetailBuilder);
	static FReply DeleteAllZones(IDetailLayoutBuilder* DetailBuilder);
	static FReply ClearArenaData(IDetailLayoutBuilder* DetailBuilder);
	static FReply DetachChildZones(IDetailLayoutBuilder* DetailBuilder);
};