#pragma once

#include "ModelViewer.h"

class AssetModelViewer :
	public ModelViewer
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

