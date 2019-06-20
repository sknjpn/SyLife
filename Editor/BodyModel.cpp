#include "BodyModel.h"

ptree BodyModel::AddToJSON(ptree pt) const
{
	// type
	pt.put("type", "Body");

	return PartModel::AddToJSON(pt);
}

void BodyModel::SetFromJSON(const ptree & pt)
{
	PartModel::SetFromJSON(pt);
}