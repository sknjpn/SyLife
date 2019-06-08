#include "SystemManager.h"

#include "ViewerManager.h"
#include "PartViewer.h"
#include "FileViewer.h"

void Main()
{
	g_systemManagerPtr = make_unique<SystemManager>();

	{
		const auto& pv = g_viewerManagerPtr->m_viewers.emplace_back(make_unique<PartViewer>());

		pv->m_drawRect = s3d::RectF(200.0, 0.0, 600.0, 600.0);
	}
	
	{
		const auto& fv = g_viewerManagerPtr->m_viewers.emplace_back(make_unique<FileViewer>());

		fv->m_drawRect = s3d::RectF(0.0, 0.0, 200.0, 600.0);
	}

	while (s3d::System::Update())
	{
		g_systemManagerPtr->Update();

		s3d::Print << s3d::Window::Size();
	}
}
