#include "PartEditor.h"

#include "PartAsset.h"

void PartEditor::reloadProperties_this()
{
	ModelEditor::reloadProperties_this();

	m_textEditState_mass.text = ToString(getModel<PartAsset>()->m_mass);
}

void PartEditor::update_this()
{
	ModelEditor::update_this();

	// mass
	SimpleGUI::TextBox(m_textEditState_mass, Vec2(8, 8));
	getModel<PartAsset>()->m_mass = Parse<double>(m_textEditState_mass.text);
	moveDrawPos(0, 48);
}
