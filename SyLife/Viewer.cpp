#include "Viewer.h"


void Viewer::UpdateAllViewers()
{
	// Viewerのリセット
	{
		auto viewers = GetRootViewer()->getAllChildViewers();
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
	}

	// destroyされたViewerの削除
	for (;;)
	{
		auto viewers = GetRootViewer()->getAllChildViewers();
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

void Viewer::Run()
{
	while (System::Update())
	{
		// ログ出力のクリア
		ClearPrint();

		UpdateAllViewers();
	}
}

const shared_ptr<Viewer>& Viewer::GetRootViewer()
{
	static shared_ptr<Viewer> rootViewer = MakeShared<Viewer>();

	return rootViewer;
}

void Viewer::destroy()
{
	m_isDestroyed = true;

	for (auto& cv : m_childViewers)
		cv->destroy();
}

Array<shared_ptr<Viewer>> Viewer::getAllChildViewers() const
{
	Array<shared_ptr<Viewer>> result;

	result.append(m_childViewers);

	for (const auto& cv : m_childViewers)
		result.append(cv->getAllChildViewers());

	return result;
}
