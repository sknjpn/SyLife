#include "ViewerManager.h"
#include "Viewer.h"

unique_ptr<ViewerManager>	g_viewerManagerPtr;

void ViewerManager::Update()
{
	shared_ptr<Viewer> mouseOverViewer = nullptr;

	for (auto it = m_viewers.rbegin(); it < m_viewers.rend(); ++it)
	{
		if ((*it)->m_drawRect.mouseOver())
		{
			mouseOverViewer = *it;

			break;
		}
	}

	for (auto it = m_viewers.begin(); it < m_viewers.end(); ++it)
	{
		auto vp = s3d::ViewportBlock2D(s3d::Rect((*it)->m_drawRect));
		auto t = s3d::Transformer2D(s3d::Mat3x2::Identity(), s3d::Mat3x2::Translate((*it)->m_drawRect.pos));

		s3d::RectF((*it)->m_drawRect.size).draw((*it)->m_backgroundColor);

		(*it)->Update(mouseOverViewer == *it);

		s3d::RectF((*it)->m_drawRect.size).drawFrame(1.0, 0.0, s3d::ColorF(s3d::Palette::Red, 0.5));
	}
}
