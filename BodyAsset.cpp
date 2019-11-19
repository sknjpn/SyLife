#include "BodyAsset.h"

#include "BodyState.h"
#include "BodyEditor.h"
#include "PartShapeViewer.h"

void BodyAsset::makeViewer()
{
	//g_viewerManagerPtr->makeViewer<BodyEditor>()->setPartAsset(shared_from_this());
	//g_viewerManagerPtr->makeViewer<PartShapeViewer>()->setPartAsset(shared_from_this());
}

shared_ptr<PartState> BodyAsset::makeState()
{
	return make_shared<BodyState>();
}

void BodyAsset::load(const JSONValue& json)
{
	PartAsset::load(json);
}

void BodyAsset::save(JSONWriter& json) const
{
	PartAsset::save(json);

	// type
	json.key(U"type").write(U"BodyAsset");
}
