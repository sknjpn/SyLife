#include "PartAsset_Needle.h"
#include "PartState_Needle.h"

shared_ptr<PartState> PartAsset_Needle::makePartState(const shared_ptr<PartConfig>& partConfig) const
{
	return make_shared<PartState_Needle>(partConfig);
}

void PartAsset_Needle::load(const JSONValue& json)
{
	PartAsset::load(json);

	m_penetrating = json[U"penetrating"].get<int>();
}

void PartAsset_Needle::save(JSONWriter& json) const
{
	PartAsset::save(json);

	json.key(U"penetrating").writeDouble(m_penetrating);
}
