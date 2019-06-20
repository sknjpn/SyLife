#include "CellModel.h"
#include <boost/property_tree/json_parser.hpp>

ptree CellModel::ToJSON() const
{
	ptree pt;

	// name
	pt.put("name", m_name);

	// body
	{
		ptree body;

		// name
		body.put("name", m_body->m_model->m_name);

		pt.add_child("body", body);
	}

	// equipments
	{
		ptree equipments;

		for (const auto& e : m_equipments)
			equipments.push_back(std::make_pair("", e->ToJSON()));

		pt.add_child("equipments", equipments);
	}

	// modules
	{
		ptree modules;

		for (const auto& m : m_modules)
			modules.push_back(std::make_pair("", m->ToJSON()));

		pt.add_child("modules", modules);
	}

	return pt;
}

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
