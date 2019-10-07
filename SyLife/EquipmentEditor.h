#pragma once

#include "PartEditor.h"

class EquipmentEditor
	: public PartEditor
{
public:
	// Reload
	void reloadProperties_this();
	void reloadProperties() override { reloadProperties_this(); }

	// Update
	void Update_this();
	void Update() override { Update_this(); }
};