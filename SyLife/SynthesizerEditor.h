#pragma once

#include "ModuleEditor.h"

class SynthesizerEditor :
	public ModuleEditor
{
public:
	// Reload
	void reloadProperties_this();
	void reloadProperties() override { reloadProperties_this(); }

	// update
	void Update_this();
	void update() override { Update_this(); }
};