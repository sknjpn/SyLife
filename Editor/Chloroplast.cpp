#include "Chloroplast.h"

#include "ViewerManager.h"
#include "AssetManager.h"

void ChloroplastModel::MakeViewers()
{
	g_viewerManagerPtr->AddViewer<ChloroplastViewer>(dynamic_pointer_cast<PartModel>(shared_from_this()));
}

shared_ptr<PartConfig> ChloroplastModel::MakePartConfig() const
{
	return make_shared<ChloroplastConfig>();
}
