#include "NeedleAsset.h"
#include "NeedleState.h"
#include "PartShapeViewer.h"

shared_ptr<PartState> NeedleAsset::makeState()
{
	return make_shared<NeedleState>();
}

void NeedleAsset::load(const JSONValue& json)
{
	PartAsset::load(json);
}

void NeedleAsset::save(JSONWriter& json) const
{
	PartAsset::save(json);

	// type
	json.key(U"type").write(U"NeedleAsset");
}
