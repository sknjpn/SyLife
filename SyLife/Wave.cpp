#include "Wave.h"

#include "ViewerManager.h"

void WaveModel::MakeViewer()
{
	g_viewerManagerPtr->MakeViewer<WaveViewer>()->SetModel(shared_from_this());
}
