#include "EditorViewer.h"
#include "AssetManager.h"
#include "GUISlider.h"

void EditorViewer::AssetList::init()
{
	setViewerSize(200, Scene::Size().y);
	setViewerPosInLocal(Scene::Size().x - 200, 0);

	// Itemの追加
	{
		const auto assets = g_assetManagerPtr->getAssets();

		for (const auto& asset : assets)
		{
			addChildViewer<Item>(asset);
		}

		// 追加ボタン用
		addChildViewer<Item>(nullptr);
	}

	addChildViewer<GUISlider>((getChildViewers<Item>().size() + 1) * 170)
		->setViewerRectInLocal(170, 0, 30, Scene::Size().y);

	// Itemの位置調整
	updateItemRects();
}

void EditorViewer::AssetList::update()
{
	// Itemの位置調整
	updateItemRects();
}

void EditorViewer::AssetList::updateItemRects() const
{
	const auto items = getChildViewers<Item>();

	Size size(170, 170);

	for (auto it = items.begin(); it != items.end(); ++it)
		(*it)->setViewerRectInLocal(0, (it - items.begin()) * size.y - getChildViewer<GUISlider>()->getDelta(), size.x, size.y);
}
