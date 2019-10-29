#include "NucleusAsset.h"

#include "NucleusState.h"
#include "NucleusEditor.h"
#include "PartShapeViewer.h"

void NucleusAsset::makeViewer()
{
	//g_viewerManagerPtr->makeViewer<NucleusEditor>()->setPartAsset(shared_from_this());
	//g_viewerManagerPtr->makeViewer<PartShapeViewer>()->setPartAsset(shared_from_this());
}

shared_ptr<PartState> NucleusAsset::makeState()
{
	return make_shared<NucleusState>();
}

void NucleusAsset::load(const ptree& pt)
{
	PartAsset::load(pt);
}

void NucleusAsset::save(ptree& pt) const
{
	PartAsset::save(pt);

	// type
	pt.put("type", "NucleusAsset");
}
