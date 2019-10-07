#pragma once

#include "AssetModelEditor.h"

class CellEditor
	: public AssetModelEditor
{
public:
	// Reload
	void reloadProperties_this();
	void reloadProperties() override { reloadProperties_this(); }

	// Update
	void Update_this();
	void Update() override { Update_this(); }
};