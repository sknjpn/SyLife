#include "PartAsset_Synthesizer.h"
#include "PartState_Synthesizer.h"
#include "World.h"
#include "ProteinAsset.h"

std::shared_ptr<PartState> PartAsset_Synthesizer::makePartState(const std::shared_ptr<PartConfig>& partConfig) const
{
	return MakeShared<PartState_Synthesizer>(partConfig);
}

void PartAsset_Synthesizer::load(const JSONValue& json)
{
	PartAsset::load(json);

	m_export = World::GetAsset<ProteinAsset>(json[U"export"].getString());
	m_productTime = json[U"productTime"].get<double>();
}

void PartAsset_Synthesizer::save(JSONWriter& json) const
{
	PartAsset::save(json);

	json.key(U"export").write(m_export->getName());
	json.key(U"productTime").writeDouble(m_productTime);
}
