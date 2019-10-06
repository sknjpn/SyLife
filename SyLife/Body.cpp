#include "Body.h"

#include "ViewerManager.h"

void BodyModel::MakeViewer()
{
	g_viewerManagerPtr->MakeViewer<BodyViewer>()->SetModel(shared_from_this());
	g_viewerManagerPtr->MakeViewer<PartShapeViewer>()->SetModel(shared_from_this());
}

shared_ptr<PartState> BodyModel::MakeState()
{
	return make_shared<BodyState>();
}
