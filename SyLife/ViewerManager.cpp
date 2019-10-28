#include "ViewerManager.h"

#include "Viewer.h"
#include "RootViewer.h"

unique_ptr<ViewerManager>	g_viewerManagerPtr;

void ViewerManager::update(const shared_ptr<Viewer>& viewer)
{
}

ViewerManager::ViewerManager()
{
	m_rootViewer = MakeShared<RootViewer>();
}

void ViewerManager::update()
{
	// ログ出力のクリア
	ClearPrint();

	// Viewerのリセット
	auto viewers = m_rootViewer->getAllChildViewers();
	shared_ptr<Viewer> mouseoverViewer;
	for (auto it = viewers.begin(); it < viewers.end(); ++it)
	{
		(*it)->m_isMouseover = false;
		(*it)->m_drawPos = Vec2::Zero();

		if ((*it)->m_viewerRect.mouseOver()) mouseoverViewer = *it;
	}

	// mouseOver 適用
	if (mouseoverViewer) mouseoverViewer->m_isMouseover = true;

	// Viewer 更新
	for (auto it = viewers.begin(); it < viewers.end(); ++it)
	{
		// 消されたならば処理しない
		if ((*it)->m_isDestroyed) continue;

		auto vp = ScopedViewport2D(Rect((*it)->m_viewerRect));
		auto t = Transformer2D(Mat3x2::Identity(), Mat3x2::Translate((*it)->m_viewerRect.pos));

		RectF((*it)->m_viewerRect.size).draw((*it)->m_backgroundColor);

		(*it)->update();

		(*it)->m_transformer.reset();

		// フレーム描画
		if (KeyF.pressed()) RectF((*it)->m_viewerRect.size).drawFrame(1.0, 0.0, ColorF(Palette::Red, 0.5));
	}

	// destroyされたViewerの削除
	{
		auto viewers = m_rootViewer->getAllChildViewers();

		for (;;)
		{
			bool flag = true;

			for (auto& v : viewers)
			{
				if (v->m_childViewers.isEmpty() && v->m_isDestroyed)
				{
					v->m_parentViewer->m_childViewers.remove(v);

					flag = false;
				}
			}

			if (flag) break;
		}
	}

}
