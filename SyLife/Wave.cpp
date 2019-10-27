#include "Wave.h"

#include "ViewerManager.h"

void WaveModel::makeViewer()
{
	g_viewerManagerPtr->makeViewer<WaveViewer>()->setPartAsset(shared_from_this());
}
