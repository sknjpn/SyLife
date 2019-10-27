#include "ModuleAsset.h"

#include "ViewerManager.h"

#include "ModuleState.h"
#include "ModuleEditor.h"
#include "PartShapeViewer.h"

void ModuleAsset::makeViewer()
{
	g_viewerManagerPtr->makeViewer<ModuleEditor>()->setPartAsset(shared_from_this());
	g_viewerManagerPtr->makeViewer<PartShapeViewer>()->setPartAsset(shared_from_this());
}

shared_ptr<PartState> ModuleAsset::makeState()
{
	return make_shared<ModuleState>();
}

void ModuleAsset::load(const ptree& pt)
{
	PartAsset::load(pt);
}

void ModuleAsset::save(ptree& pt) const
{
	PartAsset::save(pt);

	// type
	pt.put("type", "ModuleAsset");
}
