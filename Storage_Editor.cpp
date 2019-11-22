#include "Storage.h"
#include "GUIText.h"
#include "GUITextBox.h"

Storage::Editor::Editor(Storage storage)
	: m_storage(MakeUnique<Storage>(storage))
{}

Storage::Editor::Editor(function<void(const Storage&)> functionOnChanged, Storage storage)
	: m_storage(MakeUnique<Storage>(storage))
	, m_functionOnChanged(functionOnChanged)
{}

void Storage::Editor::init()
{
	addChildViewer<GUIText>(U"栄養素")
		->setViewerRectInLocal(5, 5, 100, 30);

	addChildViewer<GUITextBox>([this](const auto& s) { m_storage->m_nutrition = Parse<double>(s); m_functionOnChanged(*m_storage); }, Format(m_storage->m_nutrition))
		->setViewerRectInLocal(5, 35, 200, 50);

	addChildViewer<GUIText>(U"粒")
		->setViewerRectInLocal(5, 100, 100, 30);

	for (auto it = m_storage->begin(); it != m_storage->end(); ++it)
	{
		const auto element = *it;

		addChildViewer<Element>(element.first, element.second)
			->setViewerRectInLocal(5, 130 + 60 * double(it - m_storage->begin()), 200, 60);
	}
}

void Storage::Editor::update()
{
	RectF(getViewerSize()).rounded(5)
		.draw(Palette::White)
		.drawFrame(2.0, 0.0, Palette::Black);
}
