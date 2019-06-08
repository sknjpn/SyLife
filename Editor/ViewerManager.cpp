#include "ViewerManager.h"

unique_ptr<ViewerManager>	g_viewerManagerPtr;

void ViewerManager::Update()
{
	for (const auto& v : m_viewers)
	{
		auto t = s3d::Transformer2D(s3d::Mat3x2::Translate(v->m_drawRect.pos), true);
		
		s3d::RectF(v->m_drawRect.size).drawFrame(4.0, 0.0, s3d::Palette::Red);

		v->Update();
	}
}
