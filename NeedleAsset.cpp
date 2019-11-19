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

void NeedleAsset::load(const JSONValue& json)
{
	EquipmentAsset::load(json);
}

void NeedleAsset::save(const JSONWriter& json) const
{
	EquipmentAsset::save(json);

	// type
	//pt.put(U"type", "NeedleAsset");
}
