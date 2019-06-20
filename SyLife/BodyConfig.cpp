#include "BodyConfig.h"

#include "AssetManager.h"

ptree BodyConfig::ToJSON() const
{
	ptree pt;

	// model
	pt.put("name", m_model->m_name);

	return pt;
}

void BodyConfig::FromJSON(const ptree & pt)
{
	// model
	m_model = g_assetManagerPtr->GetBodyModel(pt.get<string>("name"));
}
