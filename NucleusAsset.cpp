#include "NucleusAsset.h"
#include "NucleusState.h"
#include "PartShapeViewer.h"

shared_ptr<PartState> NucleusAsset::makeState()
{
	return make_shared<NucleusState>();
}

void NucleusAsset::load(const JSONValue& json)
{
	PartAsset::load(json);

	m_lifespanTime = json[U"lifespanTime"].get<double>();
	m_bornTime = json[U"bornTime"].get<double>();
	m_yieldTime = json[U"yieldTime"].get<double>();
}

void NucleusAsset::save(JSONWriter& json) const
{
	PartAsset::save(json);

	// time
	json.key(U"lifespanTime").write(m_lifespanTime);
	json.key(U"bornTime").write(m_bornTime);
	json.key(U"yieldTime").write(m_yieldTime);
}
