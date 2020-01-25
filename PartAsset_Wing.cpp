#include "PartAsset_Wing.h"
#include "PartState_Wing.h"

std::shared_ptr<PartState> PartAsset_Wing::makePartState(const std::shared_ptr<PartConfig>& partConfig) const
{
	return MakeShared<PartState_Wing>(partConfig);
}

void PartAsset_Wing::load(const JSONValue& json)
{
	PartAsset::load(json);

	m_isRight = json[U"isRight"].get<bool>();
	m_strength = json[U"strength"].get<double>();
}

void PartAsset_Wing::save(JSONWriter& json) const
{
	PartAsset::save(json);

	json.key(U"isRight").writeBool(m_isRight);
	json.key(U"strength").writeDouble(m_strength);
}
