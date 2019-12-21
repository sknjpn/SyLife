#include "EditorViewer.h"
#include "GUIText.h"
#include "GUIAssetLinker.h"
#include "Asset.h"

void EditorViewer::AssetList::Item::init()
{
	if (m_asset)
	{
		addChildViewer<GUIText>(m_asset->getName())
			->setViewerRectInLocal(5, 5, 160, 30);
	}
	else
	{
		addChildViewer<GUIText>(U"新規作成")
			->setViewerRectInLocal(5, 70, 160, 30);
	}
}

void EditorViewer::AssetList::Item::update()
{
	if (isMouseover() && MouseL.down())
		getParentViewer<AssetList>()->setSelectedAsset(m_asset);

	RectF(getViewerSize()).rounded(5).draw(ColorF(0.75)).drawFrame(2.0, 0.0, Palette::Black);

	// Icon
	if (m_asset)
	{
		setDrawPos(20, 35);

		// Iconのサイズは1.0, 1.0なので、拡大を行う
		const auto t = Transformer2D(Mat3x2::Scale(130.0));

		m_asset->drawIcon();
	}

	// Select
	{
		setDrawPos(0, 0);

		if (MouseL.up())
		{
			m_isGrabbed = false;

			if (isMouseover())
			{
				const auto editViewer = getParentViewer<AssetList>()->getParentViewer<EditorViewer>();

				if (editViewer->hasChildViewer<AssetEditor>()) editViewer->getChildViewer<AssetEditor>()->destroy();

				for (const auto item : getParentViewer<AssetList>()->getChildViewers<Item>())
					item->m_isSelected = false;

				m_isSelected = true;

				m_asset->makeEditor(editViewer);
			}
		}

		if (m_isSelected)
		{
			RectF(getViewerSize()).rounded(5).draw(ColorF(Palette::Blue, 0.5));
		}
		else if (m_isGrabbed)
		{
			RectF(getViewerSize()).rounded(5).draw(ColorF(0.0, 0.5));

			if (!RectF(getViewerSize()).rounded(5).mouseOver())
			{
				if (!GetRootViewer()->hasChildViewer<GUIAssetLinker::Dragger>())
					GetRootViewer()->addChildViewer<GUIAssetLinker::Dragger>(m_asset);
			}
			else
			{
				if (GetRootViewer()->hasChildViewer<GUIAssetLinker::Dragger>())
					GetRootViewer()->getChildViewer<GUIAssetLinker::Dragger>()->destroy();
			}
		}
		else
		{
			if (isMouseover())
			{
				RectF(getViewerSize()).rounded(5).draw(ColorF(1.0, 0.5));

				if (MouseL.down()) m_isGrabbed = true;
			}
		}
	}
}