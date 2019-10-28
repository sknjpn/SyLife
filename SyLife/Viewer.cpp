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
