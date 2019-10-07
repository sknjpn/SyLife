#include "ModuleModel.h"

#include "ViewerManager.h"

void ModuleModel::makeViewer()
{
	g_viewerManagerPtr->makeViewer<ModuleEditor>()->setModel(shared_from_this());
	g_viewerManagerPtr->makeViewer<PartShapeViewer>()->setModel(shared_from_this());
}

shared_ptr<PartState> ModuleModel::MakeState()
{
	return make_shared<ModuleState>();
}
