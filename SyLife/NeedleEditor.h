#pragma once

#include "EquipmentEditor.h"

class NeedleEditor :
	public EquipmentEditor
{
public:
	// Reload
	void reloadProperties_this()
	{
		EquipmentEditor::reloadProperties_this();
	}
	void reloadProperties() override { reloadProperties_this(); }

	// Update
	void Update_this()
	{
		EquipmentEditor::Update_this();
	}
	void Update() override { Update_this(); }
};
