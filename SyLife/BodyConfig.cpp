#include "BodyConfig.h"

#include "AssetManager.h"

void BodyConfig::FromJSON(const ptree & pt)
{
	// model
	m_model = g_assetManagerPtr->GetBodyModel(pt.get<string>("name"));
}
