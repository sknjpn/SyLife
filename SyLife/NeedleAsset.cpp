#include "NeedleAsset.h"



#include "NeedleState.h"
#include "NeedleEditor.h"
#include "PartShapeViewer.h"

void NeedleAsset::makeViewer()
{
	//g_viewerManagerPtr->makeViewer<NeedleEditor>()->setPartAsset(shared_from_this());
	//g_viewerManagerPtr->makeViewer<PartShapeViewer>()->setPartAsset(shared_from_this());
}

shared_ptr<PartState> NeedleAsset::makeState()
{
	return make_shared<NeedleState>();
}

void NeedleAsset::load(const ptree& pt)
{
	EquipmentAsset::load(pt);
}

void NeedleAsset::save(ptree& pt) const
{
	EquipmentAsset::save(pt);

	// type
	pt.put("type", "NeedleAsset");
}
