#include "Body.h"
#include "AssetManager.h"

void BodyModel::SetFromJSON(const ptree & pt)
{
	PartModel::SetFromJSON(pt);
}

void BodyConfig::SetFromJSON(const ptree & pt)
{
	// model
	m_model = g_assetManagerPtr->GetModel<BodyModel>(pt.get<string>("name"));

	PartConfig::SetFromJSON(pt);
}
