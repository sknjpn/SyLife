#include "AssetModelViewer.h"
#include "AssetModel.h"

void AssetModelViewer::reloadProperties_this()
{
	// name
	m_textEditState_name.text = Unicode::Widen(getModel<AssetModel>()->getName());
}

void AssetModelViewer::Update_this()
{
	// name
	SimpleGUI::TextBox(m_textEditState_name, Vec2(8, 8), 240);
	getModel<AssetModel>()->setName(Unicode::Narrow(m_textEditState_name.text));
	MoveDrawPos(0, 48);
}
