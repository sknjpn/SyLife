#include "ModuleModel.h"

ptree ModuleModel::AddToJSON(ptree pt) const
{
	// type
	pt.put("type", "Module");

	return PartModel::AddToJSON(pt);
}

void ModuleModel::SetFromJSON(const ptree & pt)
{
	PartModel::SetFromJSON(pt);
}
