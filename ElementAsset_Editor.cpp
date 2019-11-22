#include "ElementAsset.h"
#include "GUIText.h"
#include "GUIColorPicker.h"
#include "GUITextBox.h"

void ElementAsset::Editor::init()
{
	AssetEditor::init();

	addChildViewer<GUIText>(U"名前")
		->setViewerRectInLocal(10, 10, 100, 40);
	addChildViewer<GUITextBox>([this](const String& text) { m_elementAsset->setName(text); }, m_elementAsset->getName())
		->setViewerRectInLocal(10, 50, 200, 50);

	addChildViewer<GUIText>(U"半径")
		->setViewerRectInLocal(10, 110, 100, 40);
	addChildViewer<GUITextBox>([this](const String& text) { m_elementAsset->m_radius = Parse<double>(text); }, Format(m_elementAsset->m_radius))
		->setViewerRectInLocal(10, 150, 200, 50);

	addChildViewer<GUIText>(U"色")
		->setViewerRectInLocal(10, 210, 100, 40);
	addChildViewer<GUIColorPicker>([this](const Color& color) { m_elementAsset->m_color=color; }, m_elementAsset->m_color)
		->setViewerRectInLocal(10, 250, 170, 130);

	addChildViewer<GUIText>(U"素材")
		->setViewerRectInLocal(10, 410, 100, 40);
	addChildViewer<Storage::Editor>([this](const Storage& storage) { m_elementAsset->m_material = storage; }, m_elementAsset->m_material)
		->setViewerRectInLocal(10, 450, 250, 400);
}

void ElementAsset::Editor::update()
{

}
