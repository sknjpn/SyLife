#include "SynthesizerAsset.h"
#include "SynthesizerState.h"
#include "World.h"
#include "ElementAsset.h"
#include "PartShapeViewer.h"

shared_ptr<PartState> SynthesizerAsset::makeState()
{
	return make_shared<SynthesizerState>();
}

void SynthesizerAsset::load(const JSONValue& json)
{
	PartAsset::load(json);

	// export
	m_export = World::GetInstance()->getAssets().getAsset<ElementAsset>(json[U"export"].getString());
}

void SynthesizerAsset::save(JSONWriter& json) const
{
	PartAsset::save(json);

	// export
	json.key(U"export").write(m_export->getName());
}
