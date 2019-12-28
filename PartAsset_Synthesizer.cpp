#include "PartAsset_Synthesizer.h"
#include "PartState_Synthesizer.h"
#include "World.h"
#include "ElementAsset.h"

shared_ptr<PartState> PartAsset_Synthesizer::makeState()
{
	return make_shared<PartState_Synthesizer>();
}

void PartAsset_Synthesizer::load(const JSONValue& json)
{
	PartAsset::load(json);

	// export
	m_export = World::GetAsset<ElementAsset>(json[U"export"].getString());
}

void PartAsset_Synthesizer::save(JSONWriter& json) const
{
	PartAsset::save(json);

	// export
	json.key(U"export").write(m_export->getName());
}
