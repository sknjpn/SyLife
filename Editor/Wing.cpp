#include "Wing.h"

#include "ViewerManager.h"
#include "AssetManager.h"

void WingModel::MakeViewers()
{
	g_viewerManagerPtr->AddViewer<WingViewer>(dynamic_pointer_cast<PartModel>(shared_from_this()));
}

shared_ptr<PartConfig> WingModel::MakePartConfig() const
{
	return make_shared<WingConfig>();
}
