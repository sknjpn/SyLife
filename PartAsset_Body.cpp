#include "PartAsset_Body.h"
#include "PartState_Body.h"

std::shared_ptr<PartState> PartAsset_Body::makePartState(const std::shared_ptr<PartConfig>& partConfig) const
{
	return MakeShared<PartState_Body>(partConfig);
}

void PartAsset_Body::load(const JSONValue& json)
{
	PartAsset::load(json);
}

void PartAsset_Body::save(JSONWriter& json) const
{
	PartAsset::save(json);
}
