#include "Eye.h"

#include "ViewerManager.h"

void EyeModel::makeViewer()
{
	g_viewerManagerPtr->makeViewer<EyeViewer>()->setModel(shared_from_this());
	g_viewerManagerPtr->makeViewer<PartShapeViewer>()->setModel(shared_from_this());
}

shared_ptr<PartState> EyeModel::MakeState()
{
	return make_shared<EyeState>();
}
