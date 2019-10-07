#include "EquipmentModel.h"
#include "EquipmentState.h"
#include "EquipmentEditor.h"

#include "ViewerManager.h"

void EquipmentModel::makeViewer()
{
	g_viewerManagerPtr->makeViewer<EquipmentEditor>()->setModel(shared_from_this());
	g_viewerManagerPtr->makeViewer<PartShapeViewer>()->setModel(shared_from_this());
}

shared_ptr<PartState> EquipmentModel::makeState()
{
	return make_shared<EquipmentState>();
}

void EquipmentModel::load_this(const ptree& pt)
{
	PartModel::load_this(pt);
}

void EquipmentModel::save_this(ptree& pt) const
{
	PartModel::save_this(pt);

	// type
	pt.put("type", "EquipmentModel");
}
