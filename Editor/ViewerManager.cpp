#include "ViewerManager.h"
#include "Viewer.h"

unique_ptr<ViewerManager>	g_viewerManagerPtr;

void ViewerManager::Update()
{
	for(auto it = m_viewers.begin(); it < m_viewers.end(); ++it)
	{
		const auto& v = *it;

		Graphics2D::SetViewport(Rect(v->m_drawRect));
		auto t1 = Transformer2D(Mat3x2::Translate(v->m_drawRect.pos), true);
		auto t2 = Transformer2D(Mat3x2::Translate(-v->m_drawRect.pos), false);

		RectF(v->m_drawRect.size).draw(v->m_backgroundColor);

		v->Update();

		RectF(v->m_drawRect.size).drawFrame(1.0, 0.0, ColorF(Palette::Red, 0.5));
	}
}
