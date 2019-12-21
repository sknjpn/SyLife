#include "PartAsset_Body.h"
#include "PartState_Body.h"

shared_ptr<PartState> PartAsset_Body::makeState()
{
	return make_shared<PartState_Body>();
}

void PartAsset_Body::load(const JSONValue& json)
{
	PartAsset::load(json);
}

void PartAsset_Body::save(JSONWriter& json) const
{
	PartAsset::save(json);
}
