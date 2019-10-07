﻿#pragma once

#include "ModuleEditor.h"

class SynthesizerEditor :
	public ModuleEditor
{
public:
	// Reload
	void reloadProperties_this()
	{
		ModuleEditor::reloadProperties_this();
	}
	void reloadProperties() override { reloadProperties_this(); }

	// Update
	void Update_this()
	{
		ModuleEditor::Update_this();
	}
	void Update() override { Update_this(); }
};