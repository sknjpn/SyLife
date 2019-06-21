#include "BodyConfig.h"

#include "AssetManager.h"

void BodyConfig::SetFromJSON(const ptree & pt)
{
	// model
	m_model = g_assetManagerPtr->GetBodyModel(pt.get<string>("name"));

	PartConfig::SetFromJSON(pt);
}
