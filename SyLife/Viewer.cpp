#include "Viewer.h"
#include "ViewerManager.h"

Viewer::Viewer()
{
	g_viewerManagerPtr->registerViewer(this);
}

Viewer::~Viewer()
{
	g_viewerManagerPtr->deregisterViewer(this);
}

Array<shared_ptr<Viewer>> Viewer::getAllChildren() const
{
	Array<shared_ptr<Viewer>> result;

	result.emplace_back(m_childViewers);

	for (const auto& cv : m_childViewers)
		result.append(cv->getAllChildren());

	return result;
}
