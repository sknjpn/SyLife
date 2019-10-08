#pragma once

#include "ModelEditor.h"

class AssetEditor :
	public ModelEditor
{
	TextEditState		m_textEditState_name;

public:
	// Reload
	void reloadProperties_this();
	void reloadProperties() override { reloadProperties_this(); }

	// update
	void	update_this();
	void	update() override { update_this(); }
};

