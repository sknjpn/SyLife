#include "Asset_Editor.h"
#include "Asset.h"
#include "GUIText.h"
#include "GUITextBox.h"

void Asset_Editor::init()
{
	// 名前
	addChildViewer<GUIText>(U"名前", Font(18), GUIText::Mode::DrawLeftCenter)
		->setViewerRectInLocal(10, 10, 50, 30);
	addChildViewer<GUITextBox>()

}

void Asset_Editor::update()
{
}

void Asset_Editor::apply() const
{
	//m_asset->setName(getChildViewer<GUITextBox>(U"name")->getv);
	//m_asset->setNameJP()
}
