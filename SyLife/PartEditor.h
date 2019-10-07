#pragma once

#include "AssetModelEditor.h"

class PartEditor :
	public AssetModelEditor
{
	TextEditState		m_textEditState_mass;

public:
	// Reload
	void reloadProperties_this()
	{
		ModelEditor::reloadProperties_this();

		m_textEditState_mass.text = ToString(getModel<PartModel>()->m_mass);
	}
	void reloadProperties() override { reloadProperties_this(); }

	// Update
	void Update_this()
	{
		ModelEditor::Update_this();

		// mass
		SimpleGUI::TextBox(m_textEditState_mass, Vec2(8, 8));
		getModel<PartModel>()->m_mass = Parse<double>(m_textEditState_mass.text);
		MoveDrawPos(0, 48);
	}
	void Update() override { Update_this(); }
};