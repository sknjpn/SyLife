#include "AssetEditor.h"
#include "Asset.h"

void AssetEditor::reloadProperties_this()
{
	// name
	m_textEditState_name.text = Unicode::Widen(getModel<Asset>()->getName());
}

void AssetEditor::Update_this()
{
	// name
	SimpleGUI::TextBox(m_textEditState_name, Vec2(8, 8), 240);
	getModel<Asset>()->setName(Unicode::Narrow(m_textEditState_name.text));
	MovedrawPos(0, 48);
}
