#include "NeedleModel.h"
#include "NeedleState.h"
#include "NeedleEditor.h"

#include "ViewerManager.h"

void NeedleModel::makeViewer()
{
	g_viewerManagerPtr->makeViewer<NeedleViewer>()->setModel(shared_from_this());
	g_viewerManagerPtr->makeViewer<PartShapeViewer>()->setModel(shared_from_this());
}

shared_ptr<PartState> NeedleModel::makeState()
{
	return make_shared<NeedleState>();
}

void NeedleModel::load_this(const ptree& pt)
{
	EquipmentModel::load_this(pt);
}

void NeedleModel::save_this(ptree& pt) const
{
	EquipmentModel::save_this(pt);

	// type
	pt.put("type", "NeedleModel");
}
