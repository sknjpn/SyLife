#include "Eye.h"

#include "ViewerManager.h"

void EyeModel::MakeViewer()
{
	g_viewerManagerPtr->MakeViewer<EyeViewer>()->SetModel(shared_from_this());
	g_viewerManagerPtr->MakeViewer<PartShapeViewer>()->SetModel(shared_from_this());
}

shared_ptr<PartState> EyeModel::MakeState()
{
	return make_shared<EyeState>();
}
