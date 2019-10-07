#pragma once

#include "EquipmentEditor.h"

class WingEditor :
	public EquipmentEditor
{
public:
	// Reload
	void reloadProperties_this()
	{
		EquipmentViewer::reloadProperties_this();
	}
	void reloadProperties() override { reloadProperties_this(); }

	// Update
	void Update_this()
	{
		EquipmentViewer::Update_this();
	}
	void Update() override { Update_this(); }
};
