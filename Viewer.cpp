#include "Viewer.h"

shared_ptr<Viewer>	Viewer::g_mouseoveredViewer;

void Viewer::UpdateAllViewers()
{
	// MouseOverの計算
	{
		const auto viewers = GetRootViewer()->getAllChildViewers();
		g_mouseoveredViewer = nullptr;
		for (auto it = viewers.begin(); it < viewers.end(); ++it)
			if ((*it)->getViewport() && (*it)->getViewport().value().mouseOver()) g_mouseoveredViewer = *it;
	}

	// Viewer 更新
	Viewer::GetRootViewer()->process();

	// destroyされたViewerの削除
	GetRootViewer()->removeDeadViewer();
}

void Viewer::process()
{
	if (m_isDestroyed) return;

	// 自身の更新
	if (!m_isRoot)
	{
		ScopedViewport2D sv;
		Transformer2D t;

		if (getViewport())
		{
			const auto delta = getViewerPosInWorld() - getViewport().value().pos;

			sv = getViewport() ? ScopedViewport2D(getViewport()) : ScopedViewport2D(Rect());
			t = Transformer2D(Mat3x2::Translate(delta), Mat3x2::Translate(getViewerPosInWorld() + delta));
		}
		else
		{
			sv = ScopedViewport2D(Rect());
		}

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
		m_childViewers[i]->process();
}

void Viewer::removeDeadViewer()
{
	for (const auto& cv : m_childViewers)
		cv->removeDeadViewer();

	m_childViewers.remove_if([](const auto& cv) { return cv->m_isDestroyed; });

	if (m_isDestroyed) m_parentViewer = nullptr;
}

Optional<Rect> Viewer::getViewport() const
{
	auto tRect = Rect(getViewerRectInWorld());

	if (!m_parentViewer) return tRect;
	if (!m_parentViewer->getViewport()) return none;

	auto pRect = m_parentViewer->getViewport().value();

	if (pRect.contains(tRect)) return tRect;
	if (!pRect.intersects(tRect)) return none;

	if (tRect.tl().x < pRect.tl().x)
	{
		tRect.size.x -= (pRect.tl().x - tRect.tl().x);
		tRect.pos.x += (pRect.tl().x - tRect.tl().x);
	}

	if (tRect.tl().y < pRect.tl().y)
	{
		tRect.size.y -= (pRect.tl().y - tRect.tl().y);
		tRect.pos.y += (pRect.tl().y - tRect.tl().y);
	}

	if (pRect.br().x < tRect.br().x)
		tRect.size.x -= tRect.br().x - pRect.br().x;

	if (pRect.br().y < tRect.br().y)
		tRect.size.y -= tRect.br().y - pRect.br().y;

	return tRect;
}

bool Viewer::isMouseover() const
{
	if (shared_from_this() == g_mouseoveredViewer) return true;

	for (const auto& cv : m_childViewers)
		if (cv->isMouseover()) return true;

	return false;
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

	rootViewer->m_isRoot = true;

	return rootViewer;
}

void Viewer::destroy()
{
	if (m_isRoot) return;

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
