#include "Part_WingAsset.h"
#include "Part_WingState.h"

std::shared_ptr<PartState> Part_WingAsset::makePartState(const std::shared_ptr<PartConfig>& partConfig) const
{
	return MakeShared<Part_WingState>(partConfig);
}

void Part_WingAsset::load(const JSONValue& json)
{
	PartAsset::load(json);

	m_isRight = json[U"isRight"].get<bool>();
	m_strength = json[U"strength"].get<double>();
}

void Part_WingAsset::save(JSONWriter& json) const
{
	PartAsset::save(json);

	json.key(U"isRight").writeBool(m_isRight);
	json.key(U"strength").writeDouble(m_strength);
}
