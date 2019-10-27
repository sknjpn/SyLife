#include "EquipmentAsset.h"

#include "ViewerManager.h"

#include "EquipmentState.h"
#include "EquipmentEditor.h"
#include "PartShapeViewer.h"

void EquipmentAsset::makeViewer()
{
	g_viewerManagerPtr->makeViewer<EquipmentEditor>()->setPartAsset(shared_from_this());
	g_viewerManagerPtr->makeViewer<PartShapeViewer>()->setPartAsset(shared_from_this());
}

shared_ptr<PartState> EquipmentAsset::makeState()
{
	return make_shared<EquipmentState>();
}

void EquipmentAsset::load_this(const ptree& pt)
{
	PartAsset::load_this(pt);
}

void EquipmentAsset::save_this(ptree& pt) const
{
	PartAsset::save_this(pt);

	// type
	pt.put("type", "EquipmentAsset");
}
