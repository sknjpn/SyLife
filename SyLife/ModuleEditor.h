#pragma once

#include "PartEditor.h"

class ModuleEditor :
	public PartEditor
{
public:
	// Reload
	void reloadProperties_this()
	{
		PartEditor::reloadProperties_this();
	}
	void reloadProperties() override { reloadProperties_this(); }

	// Update
	void Update_this()
	{
		PartEditor::Update_this();
	}
	void Update() override { Update_this(); }
};