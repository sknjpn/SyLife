#include "SystemManager.h"

#include "ViewerManager.h"
#include "PartViewer.h"
#include "FileViewer.h"

void Main()
{
	g_systemManagerPtr = make_unique<SystemManager>();

	{
		const auto& pv = g_viewerManagerPtr->m_viewers.emplace_back(make_unique<PartViewer>());

		//pv->m_drawRect = RectF(120.0, 0.0, )
	}
	
	{
		const auto& fv = g_viewerManagerPtr->m_viewers.emplace_back(make_unique<FileViewer>());
	}

	while (s3d::System::Update())
	{
		g_systemManagerPtr->Update();
	}
}
