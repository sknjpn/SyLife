#include "ViewerManager.h"
#include "Viewer.h"

unique_ptr<ViewerManager>	g_viewerManagerPtr;

void ViewerManager::Update()
{
	for (auto it = m_viewers.begin(); it < m_viewers.end(); ++it)
	{
		const auto& v = *it;
		auto vp = s3d::ViewportBlock2D(s3d::Rect(v->m_drawRect));
		auto t = s3d::Transformer2D(s3d::Mat3x2::Identity(), s3d::Mat3x2::Translate(v->m_drawRect.pos));

		v->Update();

		s3d::RectF(v->m_drawRect.size).drawFrame(1.0, 0.0, s3d::ColorF(s3d::Palette::Red, 0.5));
	}
}
