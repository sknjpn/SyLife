#include "CellModel.h"
#include <boost/property_tree/json_parser.hpp>


void CellModel::FromJSON(const ptree& pt)
{
	// name
	m_name = pt.get<string>("name");

	// body
	m_body->FromJSON(pt.get_child("body"));

	// equipments
	for (auto equipment : pt.get_child("equipments"))
		m_equipments.emplace_back(make_shared<EquipmentConfig>(equipment.second));

	// modules
	for (auto module : pt.get_child("modules"))
		m_modules.emplace_back(make_shared<ModuleConfig>(module.second));
}
