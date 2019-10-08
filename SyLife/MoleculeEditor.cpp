#include "stdafx.h"
#include "MoleculeEditor.h"

void MoleculeEditor::reloadProperties_this()
{
	ModelEditor::reloadProperties_this();

	m_textEditState_mass.text = ToString(getModel<MoleculeAsset>()->m_mass);
}

void MoleculeEditor::Update_this()
{
	ModelEditor::Update_this();

	// mass
	SimpleGUI::TextBox(m_textEditState_mass, Vec2(8, 8));
	getModel<MoleculeAsset>()->m_mass = Parse<double>(m_textEditState_mass.text);
	MoveDrawPos(0, 48);
}
