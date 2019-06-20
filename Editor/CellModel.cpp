#include "CellModel.h"
#include <boost/property_tree/json_parser.hpp>

ptree CellModel::ToJSON() const
{
	ptree pt;

	// name
	pt.put("name", m_name);

	// body
	if(m_body != nullptr)
	{
		ptree body;

		// name
		body.put("name", m_body->m_model->m_name);

		pt.add_child("body", body);
	}

	// modules
	{
		ptree modules;

		for (const auto& m : m_modules)
			modules.push_back(std::make_pair("", m->ToJSON()));

		pt.add_child("modules", modules);
	}

	// equipments
	{
		ptree equipments;

		for (const auto& e : m_equipments)
			equipments.push_back(std::make_pair("", e->ToJSON()));

		pt.add_child("equipments", equipments);
	}

	return pt;
}

void CellModel::FromJSON(const ptree& pt)
{
	// name
	m_name = pt.get<string>("name");
}
