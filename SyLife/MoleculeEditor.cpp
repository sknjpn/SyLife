﻿#include "stdafx.h"
#include "ElementEditor.h"

void ElementEditor::reloadProperties_this()
{
	ModelEditor::reloadProperties_this();

	m_textEditState_mass.text = ToString(getModel<ElementAsset>()->m_mass);
}

void ElementEditor::Update_this()
{
	ModelEditor::Update_this();

	// mass
	SimpleGUI::TextBox(m_textEditState_mass, Vec2(8, 8));
	getModel<ElementAsset>()->m_mass = Parse<double>(m_textEditState_mass.text);
	MoveDrawPos(0, 48);
}