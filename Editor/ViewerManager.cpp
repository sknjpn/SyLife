#include "ViewerManager.h"

unique_ptr<ViewerManager>	g_viewerManagerPtr;

void ViewerManager::Update()
{
	for (const auto& v : m_viewers)
	{
		s3d::Graphics2D::SetViewport(s3d::Rect(v->m_drawRect));
		auto t1 = s3d::Transformer2D(s3d::Mat3x2::Translate(v->m_drawRect.pos), true);
		auto t2 = s3d::Transformer2D(s3d::Mat3x2::Translate(-v->m_drawRect.pos), false);

		v->Update();

		s3d::RectF(v->m_drawRect.size).drawFrame(2.0, 0.0, s3d::Palette::Red);
	}
}
