#pragma once

#include "ModelEditor.h"

class AssetModelEditor :
	public ModelEditor
{
	TextEditState		m_textEditState_name;

public:
	// Reload
	void reloadProperties_this();
	void reloadProperties() override { reloadProperties_this(); }

	// Update
	void	Update_this();
	void	Update() override { Update_this(); }
};

