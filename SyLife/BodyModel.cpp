#include "BodyModel.h"
#include "BodyState.h"
#include "BodyEditor.h"

#include "ViewerManager.h"

void BodyModel::makeViewer()
{
	g_viewerManagerPtr->makeViewer<BodyEditor>()->setModel(shared_from_this());
	g_viewerManagerPtr->makeViewer<PartShapeViewer>()->setModel(shared_from_this());
}

shared_ptr<PartState> BodyModel::MakeState()
{
	return make_shared<BodyState>();
}
