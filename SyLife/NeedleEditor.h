#pragma once

#include "EquipmentEditor.h"

class NeedleEditor :
	public EquipmentEditor
{
public:
	// Reload
	void reloadProperties_this();
	void reloadProperties() override { reloadProperties_this(); }

	// update
	void Update_this();
	void update() override { Update_this(); }
};
