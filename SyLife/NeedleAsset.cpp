#include "NeedleAsset.h"

#include "ViewerManager.h"

#include "NeedleState.h"
#include "NeedleEditor.h"
#include "PartShapeViewer.h"

void NeedleAsset::makeViewer()
{
	g_viewerManagerPtr->makeViewer<NeedleEditor>()->setPartAsset(shared_from_this());
	g_viewerManagerPtr->makeViewer<PartShapeViewer>()->setPartAsset(shared_from_this());
}

shared_ptr<PartState> NeedleAsset::makeState()
{
	return make_shared<NeedleState>();
}

void NeedleAsset::load_this(const ptree& pt)
{
	EquipmentAsset::load_this(pt);
}

void NeedleAsset::save_this(ptree& pt) const
{
	EquipmentAsset::save_this(pt);

	// type
	pt.put("type", "NeedleAsset");
}
