#include "BodyConfig.h"
#include "AssetManager.h"

ptree BodyConfig::AddToJSON(ptree pt) const
{
	// type
	pt.put("type", "Body");

	return PartConfig::AddToJSON(pt);
}

void BodyConfig::SetFromJSON(const ptree & pt)
{
	// model
	m_model = g_assetManagerPtr->GetBodyModel(pt.get<string>("name"));

	PartConfig::SetFromJSON(pt);
}
