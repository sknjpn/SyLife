#include "Storage.h"
#include "ElementAsset.h"
#include "GUIAssetLinker.h"
#include "GUITextBox.h"

void Storage::Editor::Element::init()
{
	addChildViewer<GUIAssetLinker>([this](const auto& asset) { m_elementAsset = dynamic_pointer_cast<ElementAsset>(asset); }, m_elementAsset)
		->setViewerRectInLocal(5, 5, 90, 50);

	addChildViewer<GUITextBox>([this](const auto& text) { m_size = Parse<int>(text); }, Format(m_size))
		->setViewerRectInLocal(105, 5, 90, 50);
}

void Storage::Editor::Element::update()
{
	RectF(getViewerSize()).rounded(5).draw(Palette::White).drawFrame(2.0, 0.0, Palette::Black);
}
