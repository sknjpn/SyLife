#include "Eye.h"

#include "AssetManager.h"
#include "ViewerManager.h"

void EyeModel::MakeViewers()
{
	g_viewerManagerPtr->AddViewer<EyeViewer>(dynamic_pointer_cast<PartModel>(shared_from_this()));
}
