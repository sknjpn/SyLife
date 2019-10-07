#include "Module.h"

#include "ViewerManager.h"

void ModuleModel::makeViewer()
{
	g_viewerManagerPtr->makeViewer<ModuleViewer>()->SetModel(shared_from_this());
	g_viewerManagerPtr->makeViewer<PartShapeViewer>()->SetModel(shared_from_this());
}

shared_ptr<PartState> ModuleModel::MakeState()
{
	return make_shared<ModuleState>();
}
