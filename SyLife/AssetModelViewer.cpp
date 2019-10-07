#include "AssetModelEditor.h"
#include "AssetModel.h"

void AssetModelEditor::reloadProperties_this()
{
	// name
	m_textEditState_name.text = Unicode::Widen(getModel<AssetModel>()->getName());
}

void AssetModelEditor::Update_this()
{
	// name
	SimpleGUI::TextBox(m_textEditState_name, Vec2(8, 8), 240);
	getModel<AssetModel>()->setName(Unicode::Narrow(m_textEditState_name.text));
	MoveDrawPos(0, 48);
}
