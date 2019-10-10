#include "ElementEditor.h"

#include "ElementAsset.h"

void ElementEditor::reloadProperties_this()
{
	ModelEditor::reloadProperties_this();

	m_textEditState_mass.text = ToString(getModel<ElementAsset>()->m_mass);
}

void ElementEditor::update_this()
{
	ModelEditor::update_this();

	// mass
	SimpleGUI::TextBox(m_textEditState_mass, Vec2(8, 8));
	getModel<ElementAsset>()->m_mass = Parse<double>(m_textEditState_mass.text);
	moveDrawPos(0, 48);
}
