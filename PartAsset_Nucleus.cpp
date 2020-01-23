#include "PartAsset_Nucleus.h"
#include "PartState_Nucleus.h"

shared_ptr<PartState> PartAsset_Nucleus::makePartState(const shared_ptr<PartConfig>& partConfig) const
{
	return make_shared<PartState_Nucleus>(partConfig);
}

void PartAsset_Nucleus::load(const JSONValue& json)
{
	PartAsset::load(json);

	m_lifespanTime = json[U"lifespanTime"].get<double>();
	m_bornTime = json[U"bornTime"].get<double>();
	m_yieldTime = json[U"yieldTime"].get<double>();

	m_armor = json[U"armor"].get<int>();
}

void PartAsset_Nucleus::save(JSONWriter& json) const
{
	PartAsset::save(json);

	// time
	json.key(U"lifespanTime").write(m_lifespanTime);
	json.key(U"bornTime").write(m_bornTime);
	json.key(U"yieldTime").write(m_yieldTime);

	json.key(U"armor").write(m_armor);
}
