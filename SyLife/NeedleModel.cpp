#include "NeedleModel.h"
#include "NeedleState.h"
#include "NeedleEditor.h"

#include "ViewerManager.h"

void NeedleModel::makeViewer()
{
	g_viewerManagerPtr->makeViewer<NeedleViewer>()->setModel(shared_from_this());
	g_viewerManagerPtr->makeViewer<PartShapeViewer>()->setModel(shared_from_this());
}

shared_ptr<PartState> NeedleModel::MakeState()
{
	return make_shared<NeedleState>();
}