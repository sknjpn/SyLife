#include "BodyConfig.h"

#include "AssetManager.h"

void BodyConfig::SetFromJSON(const ptree & pt)
{
	// model
	m_model = g_assetManagerPtr->GetModel<BodyModel>(pt.get<string>("name"));

	PartConfig::SetFromJSON(pt);
}
