#include "EquipmentAsset.h"



#include "EquipmentState.h"
#include "EquipmentEditor.h"
#include "PartShapeViewer.h"

void EquipmentAsset::makeViewer()
{
	//g_viewerManagerPtr->makeViewer<EquipmentEditor>()->setPartAsset(shared_from_this());
	//g_viewerManagerPtr->makeViewer<PartShapeViewer>()->setPartAsset(shared_from_this());
}

shared_ptr<PartState> EquipmentAsset::makeState()
{
	return make_shared<EquipmentState>();
}

void EquipmentAsset::load(const JSONValue& json)
{
	PartAsset::load(json);
}

void EquipmentAsset::save(ptree& pt) const
{
	PartAsset::save(pt);

	// type
	//pt.put(U"type", "EquipmentAsset");
}
