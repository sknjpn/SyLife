#include "PartAsset_Needle.h"
#include "PartState_Needle.h"

shared_ptr<PartState> PartAsset_Needle::makeState()
{
	return make_shared<PartState_Needle>();
}

void PartAsset_Needle::load(const JSONValue& json)
{
	PartAsset::load(json);

	m_damage = json[U"damage"].get<double>();
}

void PartAsset_Needle::save(JSONWriter& json) const
{
	PartAsset::save(json);

	json.key(U"damage").writeDouble(m_damage);
}
