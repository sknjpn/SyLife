#include "EditorViewer.h"
#include "GUISlider.h"
#include "World.h"

constexpr double itemSize = 80.0;
constexpr double sliderWidth = 20.0;

void EditorViewer::AssetList::init()
{
	setViewerRectInLocal(Scene::Size().x - itemSize - sliderWidth, 0, itemSize + sliderWidth, Scene::Size().y);
	addChildViewer<GUISlider>(getChildViewers<Item>().size() * itemSize)->setViewerRectInLocal(RectF(itemSize, 0, sliderWidth, Scene::Size().y));
}

void EditorViewer::AssetList::update()
{
	const auto& items = getChildViewers<Item>();
	getChildViewer<GUISlider>()->setHeight(items.size() * itemSize);

	// 再配置
	for (auto it = items.begin(); it != items.end(); ++it)
	{
		const auto& item = *it;

		item->setViewerRectInLocal(0, (it - items.begin()) * itemSize, itemSize, itemSize);
	}
}

void EditorViewer::AssetList::addAsset(const std::shared_ptr<Asset>& asset)
{
	addChildViewer<Item>(asset);
}
