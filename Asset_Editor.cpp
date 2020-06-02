#include "Asset_Editor.h"
#include "Asset.h"
#include "GUIText.h"
#include "GUITextBox.h"

void Asset_Editor::init()
{
	// name
	{
/*		addChildViewer<GUIText>(U"名前", Font(18), GUIText::Mode::DrawLeftCenter)
			->setName(U"name")
			->setViewerRectInLocal(10, 10, 50, 30);

		addChildViewer<GUITextBox>(m_asset->getName())
			->setName(U"name")
			->setViewerRectInLocal(60, 10, 100, 30);*/
	}
}

void Asset_Editor::update()
{
}

void Asset_Editor::apply() const
{
	//m_asset->setName(getChildViewer<GUITextBox>(U"name")->getValue());
	//m_asset->setNameJP(getChildViewer<GUITextBox>(U"nameJP")->getValue());
	//m_asset->setFilePath(getChildViewer<GUITextBox>(U"filepath")->getValue());
}
