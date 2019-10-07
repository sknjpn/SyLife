#include "Eye.h"

#include "ViewerManager.h"

void EyeModel::makeViewer()
{
	g_viewerManagerPtr->makeViewer<EyeViewer>()->SetModel(shared_from_this());
	g_viewerManagerPtr->makeViewer<PartShapeViewer>()->SetModel(shared_from_this());
}

shared_ptr<PartState> EyeModel::MakeState()
{
	return make_shared<EyeState>();
}
