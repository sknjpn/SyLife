#include "AssetEditor.h"
#include "Asset.h"

void AssetEditor::reloadProperties_this()
{
	// name
	m_textEditState_name.text = getPartAsset<Asset>()->getName();
}

void AssetEditor::update_this()
{
	// name
	SimpleGUI::TextBox(m_textEditState_name, Vec2(8, 8), 240);
	getPartAsset<Asset>()->setName(m_textEditState_name.text);
	moveDrawPos(0, 48);
}
