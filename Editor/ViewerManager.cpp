#include "ViewerManager.h"

unique_ptr<ViewerManager>	g_viewerManagerPtr;

void ViewerManager::Update()
{
	for (const auto& v : m_viewers)
	{
		v->Update();
	}
}
