#include "Viewer.h"

shared_ptr<Viewer>	Viewer::g_mouseoveredViewer;

void Viewer::UpdateAllViewers()
{
	// Viewerのリセット
	{
		const auto viewers = GetRootViewer()->getAllChildViewers();
		g_mouseoveredViewer = nullptr;
		for (auto it = viewers.begin(); it < viewers.end(); ++it)
			if ((*it)->getViewerRectInWorld().mouseOver()) g_mouseoveredViewer = *it;

		// Viewer 更新
		Viewer::GetRootViewer()->process();
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

void Viewer::process()
{
	if (m_isDestroyed) return;

	// 自身の更新
	{
		const auto sv = ScopedViewport2D(Rect(getViewerRectInWorld()));
		const auto t = Transformer2D(Mat3x2::Identity(), Mat3x2::Translate(getViewerPosInWorld()));

		RectF(m_viewerRectInLocal.size).draw(m_backgroundColor);

		update();

		m_transformer.reset();
		m_drawPos = Vec2::Zero();

		// フレーム描画
		if (KeyL.pressed())
			RectF(m_viewerRectInLocal.size).drawFrame(2.0, 0.0, isMouseover() ? Palette::Red : Palette::Green);
	}

	// Childの更新
	// 長さが変わる可能性があるのでintで管理
	for (int i = 0; i < m_childViewers.size(); ++i)
	{
		const auto cv = m_childViewers[i];

		cv->process();
	}
}

Vec2 Viewer::getViewerPosInWorld() const
{
	if (m_parentViewer)
	{
		return m_viewerRectInLocal.pos.movedBy(m_parentViewer->getViewerPosInWorld());
	}
	else
	{
		return Vec2::Zero();
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
