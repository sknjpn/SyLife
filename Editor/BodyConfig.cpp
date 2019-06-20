#include "BodyConfig.h"

#include "AssetManager.h"

ptree BodyConfig::ToJSON() const
{
	return ptree();
}

void BodyConfig::FromJSON(const ptree & pt)
{
	// model
	m_model = g_assetManagerPtr->GetBodyModel(pt.get<string>("name"));
}
