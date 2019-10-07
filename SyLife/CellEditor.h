#pragma once

#include "AssetModelEditor.h"

class CellEditor
	: public AssetModelEditor
{
public:
	// Reload
	void reloadProperties_this();
	void reloadProperties() override { reloadProperties_this(); }

	// update
	void Update_this();
	void update() override { Update_this(); }
};