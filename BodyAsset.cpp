#include "BodyAsset.h"
#include "BodyState.h"

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
}
