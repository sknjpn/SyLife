#include "PartEditor.h"

void PartEditor::reloadProperties_this()
{
	ModelEditor::reloadProperties_this();

	m_textEditState_mass.text = ToString(getModel<PartAsset>()->m_mass);
}

void PartEditor::Update_this()
{
	ModelEditor::Update_this();

	// mass
	SimpleGUI::TextBox(m_textEditState_mass, Vec2(8, 8));
	getModel<PartAsset>()->m_mass = Parse<double>(m_textEditState_mass.text);
	MoveDrawPos(0, 48);
}
