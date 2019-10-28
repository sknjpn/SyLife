#include "ViewerManager.h"

#include "Viewer.h"

unique_ptr<ViewerManager>	g_viewerManagerPtr;

void ViewerManager::update()
{
	// ログ出力のクリア
	ClearPrint();

	// nullptr になったViewerのリセット
	m_viewers.remove(nullptr);

	// Priorityに応じた昇順に並び替え
	m_viewers.sort_by([](const Viewer* v1, const Viewer* v2) { return v1->getPriority() < v2->getPriority(); });

	// Viewerのリセット
	for (auto it = m_viewers.begin(); it < m_viewers.end(); ++it)
	{
		(*it)->m_isMouseOver = false;
		(*it)->m_drawPos = Vec2::Zero();
	}

	// mouseOver 適用
	for (auto it = m_viewers.rbegin(); it < m_viewers.rend(); ++it)
	{
		if (!(*it)->isInvisible() && (*it)->m_viewerRect.mouseOver())
		{
			(*it)->m_isMouseOver = true;

			break;
		}
	}

	// Viewer 更新
	for (auto it = m_viewers.begin(); it < m_viewers.end(); ++it)
	{
		// Viewerが消されてnullptrになっている可能性がある
		if (*it == nullptr) continue;

		if ((*it)->isInvisible())
		{
			auto vp = ScopedViewport2D(0, 0);

			(*it)->update();

			// Viewerが消されてnullptrになっている可能性がある
			if (*it != nullptr) (*it)->m_transformer.reset();
		}
		else
		{
			auto vp = ScopedViewport2D(Rect((*it)->m_viewerRect));
			auto t = Transformer2D(Mat3x2::Identity(), Mat3x2::Translate((*it)->m_viewerRect.pos));

			RectF((*it)->m_viewerRect.size).draw((*it)->m_backgroundColor);

			(*it)->update();

			if (*it != nullptr)
			{
				// Viewerが消されてnullptrになっている可能性がある
				(*it)->m_transformer.reset();

				// フレーム描画
				RectF((*it)->m_viewerRect.size).drawFrame(1.0, 0.0, ColorF(Palette::Red, 0.5));
			}
		}
	}
}

void ViewerManager::registerViewer(Viewer* viewer)
{
	m_viewers.emplace_back(viewer);
}

void ViewerManager::deregisterViewer( Viewer* viewer)
{
	m_viewers.remove(viewer);
}
