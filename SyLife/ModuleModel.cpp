#include "ModuleModel.h"
#include "ModuleState.h"
#include "ModuleEditor.h"

#include "ViewerManager.h"

void ModuleModel::makeViewer()
{
	g_viewerManagerPtr->makeViewer<ModuleEditor>()->setModel(shared_from_this());
	g_viewerManagerPtr->makeViewer<PartShapeViewer>()->setModel(shared_from_this());
}

shared_ptr<PartState> ModuleModel::makeState()
{
	return make_shared<ModuleState>();
}

void ModuleModel::load_this(const ptree& pt)
{
	PartModel::load_this(pt);
}

void ModuleModel::save_this(ptree& pt) const
{
	PartModel::save_this(pt);

	// type
	pt.put("type", "ModuleModel");
}
