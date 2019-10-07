#include "BodyModel.h"
#include "BodyState.h"
#include "BodyEditor.h"

#include "ViewerManager.h"

void BodyModel::makeViewer()
{
	g_viewerManagerPtr->makeViewer<BodyEditor>()->setModel(shared_from_this());
	g_viewerManagerPtr->makeViewer<PartShapeViewer>()->setModel(shared_from_this());
}

shared_ptr<PartState> BodyModel::makeState()
{
	return make_shared<BodyState>();
}

void BodyModel::load_this(const ptree& pt)
{
	PartModel::load_this(pt);
}

void BodyModel::save_this(ptree& pt) const
{
	PartModel::save_this(pt);

	// type
	pt.put("type", "BodyModel");
}
