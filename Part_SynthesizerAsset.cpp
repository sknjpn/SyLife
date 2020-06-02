#include "Part_SynthesizerAsset.h"
#include "Part_SynthesizerState.h"
#include "World.h"
#include "ProteinAsset.h"

std::shared_ptr<PartState> Part_SynthesizerAsset::makePartState(const std::shared_ptr<PartConfig>& partConfig) const
{
	return MakeShared<Part_SynthesizerState>(partConfig);
}

void Part_SynthesizerAsset::load(const JSONValue& json)
{
	PartAsset::load(json);

	m_export = World::GetAsset<ProteinAsset>(json[U"export"].getString());
	m_productTime = json[U"productTime"].get<double>();
}

void Part_SynthesizerAsset::save(JSONWriter& json) const
{
	PartAsset::save(json);

	json.key(U"export").write(m_export->getName());
	json.key(U"productTime").writeDouble(m_productTime);
}
