#include "Equipment.h"

ptree EquipmentModel::ToJSON() const
{
	ptree pt;

	// name
	pt.put("name", m_name);

	// type
	pt.put("type", "Equipment");

	// mass
	pt.put<int>("mass", m_mass);

	return pt;
}

void EquipmentModel::Load(const string & path) const
{
}

void EquipmentModel::Save() const
{
}
