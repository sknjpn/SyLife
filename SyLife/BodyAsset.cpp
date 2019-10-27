#include "BodyAsset.h"

#include "ViewerManager.h"

#include "BodyState.h"
#include "BodyEditor.h"
#include "PartShapeViewer.h"

void BodyAsset::makeViewer()
{
	g_viewerManagerPtr->makeViewer<BodyEditor>()->setPartAsset(shared_from_this());
	g_viewerManagerPtr->makeViewer<PartShapeViewer>()->setPartAsset(shared_from_this());
}

shared_ptr<PartState> BodyAsset::makeState()
{
	return make_shared<BodyState>();
}

void BodyAsset::load(const ptree& pt)
{
	PartAsset::load(pt);
}

void BodyAsset::save(ptree& pt) const
{
	PartAsset::save(pt);

	// type
	pt.put("type", "BodyAsset");
}
