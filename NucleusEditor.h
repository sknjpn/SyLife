#pragma once

#include "PartEditor.h"

class NucleusEditor :
	public PartEditor
{
public:
	// Reload
	void reloadProperties_this();
	void reloadProperties() override { reloadProperties_this(); }

	// update
	void update_this();
	void update() override { update_this(); }
};
