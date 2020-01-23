#include "PartAsset_Wing.h"
#include "PartState_Wing.h"

shared_ptr<PartState> PartAsset_Wing::makePartState(const shared_ptr<PartConfig>& partConfig) const
{
	return make_shared<PartState_Wing>(partConfig);
}

void PartAsset_Wing::load(const JSONValue& json)
{
	PartAsset::load(json);

	m_isRight = json[U"isRight"].get<bool>();
	m_penetrating = json[U"penetrating"].get<double>();
}

void PartAsset_Wing::save(JSONWriter& json) const
{
	PartAsset::save(json);

	json.key(U"isRight").writeBool(m_isRight);
	json.key(U"penetrating").writeDouble(m_penetrating);
}
