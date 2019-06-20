#include "EquipmentModel.h"

ptree EquipmentModel::AddToJSON(ptree pt) const
{
	// type
	pt.put("type", "Equipment");

	return PartModel::AddToJSON(pt);
}

void EquipmentModel::SetFromJSON(const ptree & pt)
{
	PartModel::SetFromJSON(pt);
}
