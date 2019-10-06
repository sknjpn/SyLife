#include "Module.h"

#include "ViewerManager.h"

void ModuleModel::MakeViewer()
{
	g_viewerManagerPtr->MakeViewer<ModuleViewer>()->SetModel(shared_from_this());
	g_viewerManagerPtr->MakeViewer<PartShapeViewer>()->SetModel(shared_from_this());
}

shared_ptr<PartState> ModuleModel::MakeState()
{
	return make_shared<ModuleState>();
}
