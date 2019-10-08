#pragma once

#include "AssetEditor.h"

class PartEditor :
	public AssetEditor
{
	TextEditState		m_textEditState_mass;

public:
	// Reload
	void reloadProperties_this();
	void reloadProperties() override { reloadProperties_this(); }

	// update
	void update_this();
	void update() override { update_this(); }
};