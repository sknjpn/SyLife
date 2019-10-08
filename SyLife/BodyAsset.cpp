#include "BodyAsset.h"
#include "BodyState.h"
#include "BodyEditor.h"

#include "ViewerManager.h"

void BodyAsset::makeViewer()
{
	g_viewerManagerPtr->makeViewer<BodyEditor>()->setModel(shared_from_this());
	g_viewerManagerPtr->makeViewer<PartShapeViewer>()->setModel(shared_from_this());
}

shared_ptr<PartState> BodyAsset::makeState()
{
	return make_shared<BodyState>();
}

void BodyAsset::load_this(const ptree& pt)
{
	PartAsset::load_this(pt);
}

void BodyAsset::save_this(ptree& pt) const
{
	PartAsset::save_this(pt);

	// type
	pt.put("type", "BodyAsset");
}
